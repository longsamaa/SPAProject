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
		this->lon = _node.lon;
		this->lat = _node.lat;
		adjacentNodes.clear(); 
		for (int i = 0; i < _node.adjacentNodes.size(); i++)
		{
			adjacentNodes.push_back(_node.adjacentNodes.at(i));
		}
	}
	void Node::copy_P(Node& _node)
	{
		id = _node.id; 
		this->lon = _node.lon;
		this->lat = _node.lat;
		adjacentNodes = _node.adjacentNodes; 
	}
	bool Node::compareAdjacentNodes(Long::Node _node)
	{
		if (this->adjacentNodes.size() != _node.adjacentNodes.size())
		{
			return false; 
		}
		for (int i = 0; i < this->adjacentNodes.size(); i++)
		{
			if (this->adjacentNodes.at(i)->getId() != _node.adjacentNodes.at(i)->getId())
			{
				return false; 
			}
		}
		return true; 
	}
}