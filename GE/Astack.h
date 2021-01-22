#if !defined __ASTACK__H__
#define __ASTACK__H__

namespace MapEngine
{
	template<class TYPE>
	class TAStack
	{
	public:
		TAStack(int Capacity = 32, int GrowBy = 32);
		~TAStack();
		void Empty();
		bool IsEmpty();
		void Push(const TYPE& Info);
		TYPE& Top();
		void Pop();
		TAStack<TYPE>& Copy(TAStack<TYPE>& S)
		{
			if (m_Capacity < S.m_Capacity)
			{
				delete[]m_pData;
				m_pData = new TYPE[m_Capacity];
			}
			m_Capacity = S.m_Capacity;
			m_GrowBy = S.m_GrowBy;
			m_TopOfStack = S.m_TopOfStack;
			for (int i = 0; i <= m_TopOfStack; i++)
				m_pData[i] = S.m_pData[i];
			return *this;
		}
		TAStack<TYPE>& operator =(TAStack<TYPE>& S)
		{
			return Copy(S);
		}
	private:
		TYPE* m_pData;
		int	m_TopOfStack;
		int	m_Capacity;
		int	m_GrowBy;
	};

	template<class TYPE>
	TAStack<TYPE>::TAStack(int Capacity, int GrowBy)
	{
		m_TopOfStack = -1;
		m_Capacity = Capacity;
		m_GrowBy = GrowBy;
		m_pData = new TYPE[m_Capacity];
	}

	template<class TYPE>
	TAStack<TYPE>::~TAStack()
	{
		if (m_pData)
			delete[]m_pData;
	}

	template<class TYPE>
	inline void TAStack<TYPE>::Empty()
	{
		m_TopOfStack = -1;
	}

	template<class TYPE>
	inline bool TAStack<TYPE>::IsEmpty()
	{
		return m_TopOfStack == -1;
	}

	template<class TYPE>
	inline void TAStack<TYPE>::Push(const TYPE& Info)
	{
		if (m_TopOfStack == m_Capacity - 1) // the stack is full
		{
			TYPE* pTempData;
			int Size = m_Capacity;
			m_Capacity += m_GrowBy;
			pTempData = new TYPE[m_Capacity];
			for (int i = 0; i < Size; i++)
				pTempData[i] = m_pData[i];
			delete[]m_pData;
			m_pData = pTempData;
		}
		m_pData[++m_TopOfStack] = Info;
	}

	template<class TYPE>
	inline TYPE& TAStack<TYPE>::Top()
	{
		return m_pData[m_TopOfStack];
	}

	template<class TYPE>
	inline void TAStack<TYPE>::Pop()
	{
		m_TopOfStack--;
	}
} //namespace MapEngine

#endif // !defined __ASTACK__H__
