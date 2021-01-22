#if !defined(_RTREE_H_)
#define _RTREE_H_

#include "AStack.h"

using namespace MapEngine;

template<class DATATYPE, class ELEMTYPE, int DIMS, int TMAXNODES = 7>
class RTree
{
public:
	enum
	{
		MAXNODES = TMAXNODES,		///< Max elements in node
		MINNODES = MAXNODES >> 1,	///< Min elements in node
	};
public:
	class TNode;
	struct TRect
	{
		ELEMTYPE m_Min[DIMS];	///< Min dimensions of bounding box 
		ELEMTYPE m_Max[DIMS];	///< Max dimensions of bounding box 
	};
	struct TBranch
	{
		TRect m_Rect;                                    ///< Bounds
		union
		{
			TNode* m_Child;                              ///< Child node
			DATATYPE m_Data;                            ///< Data Id or Ptr
		};
	};
	class TNode
	{
	public:
		int m_Count;	///< Count
		int m_Level;	///< Leaf is zero, others positive
		TBranch m_Branch[MAXNODES];	///< Branch
	public:
		TNode()
		{
			m_Count = 0;
			m_Level = -1;
		}
		virtual ~TNode()
		{
		}
		bool IsInternal()
		{
			return (m_Level > 0);
		}
		bool IsLeaf()
		{
			return (m_Level == 0);
		}
	};
	/// A link list of nodes for reinsertion after a delete operation
	struct ListNode
	{
		ListNode* m_next;	///< Next in list
		TNode* m_node;		///< TNode
	};
	/// Variables for finding a split partition
	class PartitionVars
	{
	public:
		int m_Partition[MAXNODES + 1];
		int m_Total;
		int m_MinFill;
		int m_Taken[MAXNODES + 1];
		int m_Count[2];
		TRect* m_Cover;
		double m_Area[2];
		TBranch m_BranchBuf[MAXNODES + 1];
		int m_BranchCount;
		TRect m_CoverSplit;
		double m_CoverSplitArea;
	public:
		PartitionVars()
		{
			m_Cover = new TRect[2];
		}
		virtual ~PartitionVars()
		{
			if (m_Cover)
				delete[]m_Cover;
		}
	};

	/// Iterator is not remove safe.
	class Iterator
	{
	private:
		class StackElement
		{
		public:
			TNode* m_node;
			int m_branchIndex;
		};
		TRect m_Overlap;
		// Decide whether two rectangles overlap. 
		bool Overlap(TRect* a_rectA, TRect* a_rectB)
		{
			for (int index = 0; index < DIMS; ++index)
			{
				if (a_rectA->m_Min[index] > a_rectB->m_Max[index] ||
					a_rectB->m_Min[index] > a_rectA->m_Max[index])
					return false;
			}
			return true;
		}
	public:
		Iterator(const ELEMTYPE a_min[DIMS], const ELEMTYPE a_max[DIMS])
		{
			for (int axis = 0; axis < DIMS; ++axis)
			{
				m_Overlap.m_Min[axis] = a_min[axis];
				m_Overlap.m_Max[axis] = a_max[axis];
			}
			m_pS = new TAStack<StackElement>(2048, 1024);
			Init();
		}
		~Iterator()
		{
			if (m_pS)
				delete m_pS;
		}
		/// Is iterator invalid
		bool IsNull()
		{
			return m_pS->IsEmpty();
		}
		/// Access the current data element. Caller must be sure iterator is not NULL first.
		DATATYPE& operator*()
		{
			StackElement curTos = Top();
			Pop();
			return curTos.m_node->m_Branch[curTos.m_branchIndex].m_Data;
		}
		/// Access the current data element. Caller must be sure iterator is not NULL first.
		const DATATYPE& operator*() const
		{
			StackElement curTos = Top();
			Pop();
			return curTos.m_node->m_Branch[curTos.m_branchIndex].m_Data;
		}
		/// Find the next data element
		bool operator++()
		{
			return FindNextData();
		}
	private:
		/// Reset iterator
		void Init()
		{
			m_pS->Empty();
		}
		// Find the next data element in the tree (For internal use only)
		bool FindNextData()
		{
			for (;;)
			{
				if (IsNull())
				{
					return false;
				}
				StackElement curTos = Top();
				if (curTos.m_node->IsLeaf())
					return true;
				else
				{
					Pop();
					TNode* nextLevelnode = curTos.m_node->m_Branch[curTos.m_branchIndex].m_Child;
					for (int i = 0; i < nextLevelnode->m_Count; i++)
					{
						if (Overlap(&m_Overlap, &(nextLevelnode->m_Branch[i].m_Rect)))
							Push(nextLevelnode, i);
					}
				}
			}
		}
		// Push node and branch onto iteration stack (For internal use only)
		void Push(TNode* a_node, int a_branchIndex)
		{
			StackElement el;
			el.m_node = a_node;
			el.m_branchIndex = a_branchIndex;
			m_pS->Push(el);
		}
		/// Top element off iteration stack (For internal use only)
		StackElement& Top()
		{
			return m_pS->Top();
		}
		/// Pop element off iteration stack (For internal use only)
		void Pop()
		{
			m_pS->Pop();
		}
	private:
		TAStack<StackElement>* m_pS;	///< Stack as we are doing iteration instead of recursion
	public:
		friend RTree<DATATYPE, ELEMTYPE, DIMS, TMAXNODES>;
	};
public:
	TNode* m_Root;	///< Root of tree

public:
	RTree()
	{
		m_Root = AllocNode();
		m_Root->m_Level = 0;
	}
	virtual ~RTree()
	{
		Reset();
	}
	void Insert(const ELEMTYPE a_min[DIMS], const ELEMTYPE a_max[DIMS], const DATATYPE& a_dataId)
	{
		TRect rect;
		for (int i = 0; i < DIMS; i++)
		{
			rect.m_Min[i] = a_min[i];
			rect.m_Max[i] = a_max[i];
		}
		InsertRect(&rect, a_dataId, &m_Root, 0);
	}
	void Remove(const ELEMTYPE a_min[DIMS], const ELEMTYPE a_max[DIMS], const DATATYPE& a_dataId)
	{
		TRect rect;
		for (int i = 0; i < DIMS; i++)
		{
			rect.m_Min[i] = a_min[i];
			rect.m_Max[i] = a_max[i];
		}
		RemoveRect(&rect, a_dataId, &m_Root);
	}
	void RemoveAll()
	{
		// Delete all existing nodes
		Reset();
		m_Root = AllocNode();
		m_Root->m_Level = 0;
	}
	/// Get 'first' for iteration
	void GetFirst(Iterator& a_it)
	{
		a_it.Init();
		if (m_Root && (m_Root->m_Count > 0))
		{
			for (int i = 0; i < m_Root->m_Count; i++)
			{
				if (Overlap(&a_it.m_Overlap, &(m_Root->m_Branch[i].m_Rect)))
					a_it.Push(m_Root, i);
			}
			a_it.FindNextData();
		}
	}
	/// Get Next for iteration
	void GetNext(Iterator& a_it)
	{
		++a_it;
	}
	/// Is iterator NULL, or at end?
	bool IsNull(Iterator& a_it)
	{
		return a_it.IsNull();
	}
	/// Get object at iterator position
	DATATYPE& GetAt(Iterator& a_it)
	{
		return *a_it;
	}
private:
	bool Overlap(TRect* a_rectA, TRect* a_rectB)
	{
		for (int index = 0; index < DIMS; ++index)
			if (a_rectA->m_Min[index] > a_rectB->m_Max[index] ||
				a_rectB->m_Min[index] > a_rectA->m_Max[index])
				return false;
		return true;
	}
	double RectVolume(TRect* a_rect)
	{
		double volume = (double)1;
		for (int index = 0; index < DIMS; ++index)
			volume *= a_rect->m_Max[index] - a_rect->m_Min[index];
		return volume;
	}
	TRect CombineRect(TRect* a_rectA, TRect* a_rectB)
	{
		TRect newRect;
		for (int index = 0; index < DIMS; ++index)
		{
			newRect.m_Min[index] = min(a_rectA->m_Min[index], a_rectB->m_Min[index]);
			newRect.m_Max[index] = max(a_rectA->m_Max[index], a_rectB->m_Max[index]);
		}
		return newRect;
	}
	int PickBranch(TRect* a_rect, TNode* a_node)
	{
		bool firstTime = true;
		double increase;
		double bestIncr = (double)-1;
		double area;
		double bestArea;
		int best;
		TRect tempRect;

		for (int index = 0; index < a_node->m_Count; ++index)
		{
			TRect* curRect = &a_node->m_Branch[index].m_Rect;
			area = RectVolume(curRect);
			tempRect = CombineRect(a_rect, curRect);
			increase = RectVolume(&tempRect) - area;
			if ((increase < bestIncr) || firstTime)
			{
				best = index;
				bestArea = area;
				bestIncr = increase;
				firstTime = false;
			}
			else if ((increase == bestIncr) && (area < bestArea))
			{
				best = index;
				bestArea = area;
				bestIncr = increase;
			}
		}
		return best;
	}
	bool AddBranch(TBranch* a_branch, TNode* a_node, TNode** a_newNode)
	{
		if (a_node->m_Count < MAXNODES)  // Split won't be necessary
		{
			a_node->m_Branch[a_node->m_Count] = *a_branch;
			++a_node->m_Count;
			return false;
		}
		else
		{
			SplitNode(a_node, a_branch, a_newNode);
			return true;
		}
	}
	void SplitNode(TNode* a_node, TBranch* a_branch, TNode** a_newNode)
	{
		// Could just use local here, but member or external is faster since it is reused
		PartitionVars localVars;
		PartitionVars* parVars = &localVars;
		int level;
		// Load all the branches into a buffer, initialize old node
		level = a_node->m_Level;
		GetBranches(a_node, a_branch, parVars);
		// Find partition
		ChoosePartition(parVars, MINNODES);
		// Put branches from buffer into 2 nodes according to chosen partition
		*a_newNode = AllocNode();
		(*a_newNode)->m_Level = a_node->m_Level = level;
		LoadNodes(a_node, *a_newNode, parVars);
	}
	void GetBranches(TNode* a_node, TBranch* a_branch, PartitionVars* a_parVars)
	{
		// Load the branch buffer
		int index;
		for (index = 0; index < MAXNODES; ++index)
		{
			a_parVars->m_BranchBuf[index] = a_node->m_Branch[index];
		}
		a_parVars->m_BranchBuf[MAXNODES] = *a_branch;
		a_parVars->m_BranchCount = MAXNODES + 1;
		// Calculate rect containing all in the set
		a_parVars->m_CoverSplit = a_parVars->m_BranchBuf[0].m_Rect;
		for (index = 1; index < MAXNODES + 1; ++index)
		{
			a_parVars->m_CoverSplit = CombineRect(&a_parVars->m_CoverSplit, &a_parVars->m_BranchBuf[index].m_Rect);
		}
		a_parVars->m_CoverSplitArea = RectVolume(&a_parVars->m_CoverSplit);
		InitNode(a_node);
	}
	void InitNode(TNode* a_node)
	{
		a_node->m_Count = 0;
		a_node->m_Level = -1;
	}
	void ChoosePartition(PartitionVars* a_parVars, int a_minFill)
	{
		double biggestDiff;
		int group, chosen, betterGroup;

		InitParVars(a_parVars, a_parVars->m_BranchCount, a_minFill);
		PickSeeds(a_parVars);

		while (((a_parVars->m_Count[0] + a_parVars->m_Count[1]) < a_parVars->m_Total)
			&& (a_parVars->m_Count[0] < (a_parVars->m_Total - a_parVars->m_MinFill))
			&& (a_parVars->m_Count[1] < (a_parVars->m_Total - a_parVars->m_MinFill)))
		{
			biggestDiff = (double)-1;
			for (int index = 0; index < a_parVars->m_Total; ++index)
			{
				if (!a_parVars->m_Taken[index])
				{
					TRect* curRect = &a_parVars->m_BranchBuf[index].m_Rect;
					TRect rect0 = CombineRect(curRect, &a_parVars->m_Cover[0]);
					TRect rect1 = CombineRect(curRect, &a_parVars->m_Cover[1]);
					double growth0 = RectVolume(&rect0) - a_parVars->m_Area[0];
					double growth1 = RectVolume(&rect1) - a_parVars->m_Area[1];
					double diff = growth1 - growth0;
					if (diff >= 0)
					{
						group = 0;
					}
					else
					{
						group = 1;
						diff = -diff;
					}
					if (diff > biggestDiff)
					{
						biggestDiff = diff;
						chosen = index;
						betterGroup = group;
					}
					else if ((diff == biggestDiff) && (a_parVars->m_Count[group] < a_parVars->m_Count[betterGroup]))
					{
						chosen = index;
						betterGroup = group;
					}
				}
			}
			Classify(chosen, betterGroup, a_parVars);
		}
		// If one group too full, put remaining rects in the other
		if ((a_parVars->m_Count[0] + a_parVars->m_Count[1]) < a_parVars->m_Total)
		{
			if (a_parVars->m_Count[0] >= a_parVars->m_Total - a_parVars->m_MinFill)
			{
				group = 1;
			}
			else
			{
				group = 0;
			}
			for (int index = 0; index < a_parVars->m_Total; ++index)
			{
				if (!a_parVars->m_Taken[index])
				{
					Classify(index, group, a_parVars);
				}
			}
		}
	}
	void InitParVars(PartitionVars* a_parVars, int a_maxRects, int a_minFill)
	{
		a_parVars->m_Count[0] = a_parVars->m_Count[1] = 0;
		a_parVars->m_Area[0] = a_parVars->m_Area[1] = (double)0;
		a_parVars->m_Total = a_maxRects;
		a_parVars->m_MinFill = a_minFill;
		for (int index = 0; index < a_maxRects; ++index)
		{
			a_parVars->m_Taken[index] = false;
			a_parVars->m_Partition[index] = -1;
		}
	}
	void Classify(int a_index, int a_group, PartitionVars* a_parVars)
	{
		a_parVars->m_Partition[a_index] = a_group;
		a_parVars->m_Taken[a_index] = true;

		if (a_parVars->m_Count[a_group] == 0)
		{
			a_parVars->m_Cover[a_group] = a_parVars->m_BranchBuf[a_index].m_Rect;
		}
		else
		{
			a_parVars->m_Cover[a_group] = CombineRect(&a_parVars->m_BranchBuf[a_index].m_Rect, &a_parVars->m_Cover[a_group]);
		}
		a_parVars->m_Area[a_group] = RectVolume(&a_parVars->m_Cover[a_group]);
		++a_parVars->m_Count[a_group];
	}
	void PickSeeds(PartitionVars* a_parVars)
	{
		int seed0, seed1;
		double worst, waste;
		double area[MAXNODES + 1];

		for (int index = 0; index < a_parVars->m_Total; ++index)
		{
			area[index] = RectVolume(&a_parVars->m_BranchBuf[index].m_Rect);
		}
		worst = -a_parVars->m_CoverSplitArea - 1;
		for (int indexA = 0; indexA < a_parVars->m_Total - 1; ++indexA)
		{
			for (int indexB = indexA + 1; indexB < a_parVars->m_Total; ++indexB)
			{
				TRect oneRect = CombineRect(&a_parVars->m_BranchBuf[indexA].m_Rect, &a_parVars->m_BranchBuf[indexB].m_Rect);
				waste = RectVolume(&oneRect) - area[indexA] - area[indexB];
				if (waste > worst)
				{
					worst = waste;
					seed0 = indexA;
					seed1 = indexB;
				}
			}
		}
		Classify(seed0, 0, a_parVars);
		Classify(seed1, 1, a_parVars);
	}
	bool InsertRect(TRect* a_rect, const DATATYPE& a_id, TNode** a_root, int a_level)
	{
		TNode* newRoot;
		TNode* newNode;
		TBranch branch;

		if (InsertRectRec(a_rect, a_id, *a_root, &newNode, a_level))  // Root split
		{
			newRoot = AllocNode();  // Grow tree taller and new root
			newRoot->m_Level = (*a_root)->m_Level + 1;
			branch.m_Rect = NodeCover(*a_root);
			branch.m_Child = *a_root;
			AddBranch(&branch, newRoot, NULL);
			branch.m_Rect = NodeCover(newNode);
			branch.m_Child = newNode;
			AddBranch(&branch, newRoot, NULL);
			*a_root = newRoot;
			return true;
		}
		return false;
	}
	bool InsertRectRec(TRect* a_rect, const DATATYPE& a_id, TNode* a_node, TNode** a_newNode, int a_level)
	{
		int index;
		TBranch branch;
		TNode* otherNode;

		// Still above level for insertion, go down tree recursively
		if (a_node->m_Level > a_level)
		{
			index = PickBranch(a_rect, a_node);
			if (!InsertRectRec(a_rect, a_id, a_node->m_Branch[index].m_Child, &otherNode, a_level))
			{
				// Child was not split
				a_node->m_Branch[index].m_Rect = CombineRect(a_rect, &(a_node->m_Branch[index].m_Rect));
				return false;
			}
			else // Child was split
			{
				a_node->m_Branch[index].m_Rect = NodeCover(a_node->m_Branch[index].m_Child);
				branch.m_Child = otherNode;
				branch.m_Rect = NodeCover(otherNode);
				return AddBranch(&branch, a_node, a_newNode);
			}
		}
		else if (a_node->m_Level == a_level) // Have reached level for insertion. Add rect, split if necessary
		{
			branch.m_Rect = *a_rect;
			branch.m_Child = (TNode*)a_id;
			// Child field of leaves contains id of data record
			return AddBranch(&branch, a_node, a_newNode);
		}
		else
		{
			return false;
		}
	}
	TNode* AllocNode()
	{
		TNode* newNode;
		newNode = new TNode;
		InitNode(newNode);
		return newNode;
	}
	TRect NodeCover(TNode* a_node)
	{
		int firstTime = true;
		TRect rect;
		InitRect(&rect);
		for (int index = 0; index < a_node->m_Count; ++index)
		{
			if (firstTime)
			{
				rect = a_node->m_Branch[index].m_Rect;
				firstTime = false;
			}
			else
			{
				rect = CombineRect(&rect, &(a_node->m_Branch[index].m_Rect));
			}
		}
		return rect;
	}
	void InitRect(TRect* a_rect)
	{
		for (int index = 0; index < DIMS; ++index)
		{
			a_rect->m_Min[index] = (ELEMTYPE)0;
			a_rect->m_Max[index] = (ELEMTYPE)0;
		}
	}
	void LoadNodes(TNode* a_nodeA, TNode* a_nodeB, PartitionVars* a_parVars)
	{
		for (int index = 0; index < a_parVars->m_Total; ++index)
		{
			if (a_parVars->m_Partition[index] == 0)
			{
				AddBranch(&a_parVars->m_BranchBuf[index], a_nodeA, NULL);
			}
			else if (a_parVars->m_Partition[index] == 1)
			{
				AddBranch(&a_parVars->m_BranchBuf[index], a_nodeB, NULL);
			}
		}
	}
	void Reset()
	{
		RemoveAllRec(m_Root);
	}
	void RemoveAllRec(TNode* a_node)
	{
		if (a_node->IsInternal()) // This is an internal node in the tree
		{
			for (int index = 0; index < a_node->m_Count; ++index)
			{
				RemoveAllRec(a_node->m_Branch[index].m_Child);
			}
		}
		FreeNode(a_node);
	}
	void FreeNode(TNode* a_node)
	{
		delete a_node;
	}
	bool RemoveRect(TRect* a_rect, const DATATYPE& a_id, TNode** a_root)
	{
		TNode* tempNode;
		ListNode* reInsertList = NULL;
		if (!RemoveRectRec(a_rect, a_id, *a_root, &reInsertList))
		{
			// Found and deleted a data item
			// Reinsert any branches from eliminated nodes
			while (reInsertList)
			{
				tempNode = reInsertList->m_node;
				for (int index = 0; index < tempNode->m_Count; ++index)
				{
					InsertRect(&(tempNode->m_Branch[index].m_Rect),
						tempNode->m_Branch[index].m_Data,
						a_root,
						tempNode->m_Level);
				}
				ListNode* remLNode = reInsertList;
				reInsertList = reInsertList->m_next;
				FreeNode(remLNode->m_node);
				FreeListNode(remLNode);
			}
			// Check for redundant root (not leaf, 1 child) and eliminate
			if ((*a_root)->m_Count == 1 && (*a_root)->IsInternal())
			{
				tempNode = (*a_root)->m_Branch[0].m_Child;
				FreeNode(*a_root);
				*a_root = tempNode;
			}
			return false;
		}
		else
		{
			return true;
		}
	}
	bool RemoveRectRec(TRect* a_rect, const DATATYPE& a_id, TNode* a_node, ListNode** a_listNode)
	{
		if (a_node->IsInternal())  // not a leaf node
		{
			for (int index = 0; index < a_node->m_Count; ++index)
			{
				if (Overlap(a_rect, &(a_node->m_Branch[index].m_Rect)))
				{
					if (!RemoveRectRec(a_rect, a_id, a_node->m_Branch[index].m_Child, a_listNode))
					{
						if (a_node->m_Branch[index].m_Child->m_Count >= MINNODES)
						{
							// child removed, just resize parent rect
							a_node->m_Branch[index].m_Rect = NodeCover(a_node->m_Branch[index].m_Child);
						}
						else
						{
							// child removed, not enough entries in node, eliminate node
							ReInsert(a_node->m_Branch[index].m_Child, a_listNode);
							DisconnectBranch(a_node, index); // Must return after this call as count has changed
						}
						return false;
					}
				}
			}
			return true;
		}
		else // A leaf node
		{
			for (int index = 0; index < a_node->m_Count; ++index)
			{
				if (a_node->m_Branch[index].m_Child == (TNode*)a_id)
				{
					DisconnectBranch(a_node, index); // Must return after this call as count has changed
					return false;
				}
			}
			return true;
		}
	}
	void FreeListNode(ListNode* a_listNode)
	{
		delete a_listNode;
	}
	void ReInsert(TNode* a_node, ListNode** a_listNode)
	{
		ListNode* newListNode;

		newListNode = AllocListNode();
		newListNode->m_node = a_node;
		newListNode->m_next = *a_listNode;
		*a_listNode = newListNode;
	}
	void DisconnectBranch(TNode* a_node, int a_index)
	{
		// Remove element by swapping with the last element to prevent gaps in array
		a_node->m_Branch[a_index] = a_node->m_Branch[a_node->m_Count - 1];
		--a_node->m_Count;
	}
	ListNode* AllocListNode()
	{
		return new ListNode;
	}
};

#endif // !defined(_RTREE_H_)

