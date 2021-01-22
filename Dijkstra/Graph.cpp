#include "Graph.h"

namespace Long
{
	void Graph::addNode(Long::Node* node)
	{
		Nodes.push_back(node); 
	}
	void Graph::addEdgeQq(int idRootNode, int idAdijacentNode, double w)
	{
		EdgesQq[idRootNode].push_back(std::make_pair(idAdijacentNode, w));
	}

	int Graph::getMaxNodeId()
	{
		int max = -1;
		for (int i = 0; i < Nodes.size(); i++)
		{
			int id = Nodes[i]->getId();
			if (id > max)
			{
				max = id;
			}
		}
		return max;
	}
	void Graph::addAdjacentNode(Node* rootNode, Node* adadjacentid)
	{
		for (int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->getId() == rootNode->getId())
			{
				Nodes[i]->addAdjacentNode(adadjacentid);
			}
			if (Nodes[i]->getId() == adadjacentid->getId())
			{
				Nodes[i]->addAdjacentNode(rootNode);
			}
		}
	}
	Node* Graph::getNode(int id)
	{
		for (int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->getId() == id)
			{
				return Nodes[i];
			}
		}
		return nullptr;
	}
	double Graph::getDistanceHaversin(double startLat, double startLong, double endLat, double endLong)
	{

		double haversine, distance;

		double dLat, dLon;

		double DEG_RAD = 0.01745329251994;

		double R_EARTH = 6367.45;

		dLat = (endLat - startLat) * DEG_RAD;

		dLon = (endLong - startLong) * DEG_RAD;

		haversine = std::sin(dLat * 0.5) * std::sin(dLat * 0.5) +

			std::sin(dLon * 0.5) * std::sin(dLon * 0.5) *

			std::cos(startLat * DEG_RAD) *

			std::cos(endLat * DEG_RAD);

		distance = std::asin(std::sqrt(haversine)) * R_EARTH * 2.0;

		return distance;

	}
	void Graph::addEdge(Long::Edge* edge)
	{
		Edges.push_back(edge); 
	}
	void Graph::deleteNode(Node* node)
	{
		for (int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->getId() == node->getId())
			{
				Nodes.erase(Nodes.begin() + i); 
				break; 
			}
		}
	}
}
