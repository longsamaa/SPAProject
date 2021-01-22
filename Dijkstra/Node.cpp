#include "Node.h"

namespace Long
{
	void Node::addAdjacentNode(Node* node)
	{
		adjacentNodes.push_back(node);
	}
	void Node::deleteAdjacentNode(Node* node)
	{
		for (int i = 0; i <= adjacentNodes.size(); i++)
		{
			if (adjacentNodes[i]->getId() == node->getId())
			{
				adjacentNodes.erase(adjacentNodes.begin() + i); 
				break; 
			}
		}
	}
	void Node::copy(Node _node)
	{
		id = _node.id;
		lon = _node.lon;
		lat = _node.lat;
		adjacentNodes = _node.adjacentNodes;
	}
	void Node::copy_P(Node& _node)
	{
		id = _node.id; 
		lon = _node.lon; 
		lat = _node.lat; 
		adjacentNodes = _node.adjacentNodes; 
	}
}