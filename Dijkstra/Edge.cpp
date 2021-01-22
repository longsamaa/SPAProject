#include "Edge.h"

namespace Long
{
	void Edge::addEdge(Node* node_1, Node* node_2)
	{
		Nodes.push_back(std::make_pair(node_1, node_2)); 
		//begin_Node = node_1; 
		//end_Node = node_2; 
	}
	void Edge::copy(Edge _e)
	{
		id = _e.id; 
		Nodes = _e.Nodes; 
	}
	void Edge::setNull()
	{
		id = -1; 
		Nodes.clear(); 
	}
	bool Edge::isEdge()
	{
		return (id >= 0) && (Nodes.size() > 0); 
	}
}
