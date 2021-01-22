#pragma once
#include "Node.h"
#include <vector>
typedef std::pair<Long::Node*, Long::Node*> epair;
namespace Long 
{
	class Edge
	{
	public: 
		Edge()
		{
			id = -1; 
			Nodes.clear(); 
	/*		begin_Node = NULL; 
			end_Node = NULL; */
		}
		Edge(Edge& _edge)
		{
			id = _edge.id; 
			Nodes = _edge.Nodes; 
		}
		void addEdge(Node* node_1, Node* node_2); 
		void copy(Edge _e);
		void setNull(); 
		bool isEdge(); 
	public: 
		int id; 
		std::vector<epair> Nodes; 
		/*Long::Node* begin_Node; 
		Long::Node* end_Node; */
	};
}

