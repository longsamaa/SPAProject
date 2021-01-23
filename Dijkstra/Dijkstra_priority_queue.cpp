#include "Dijkstra_priority_queue.h"
#define INF DBL_MAX

namespace Long
{
	void Dijkstra_priority_queue::shortestPath(Long::Graph graph,Long::Node src_Node, Long::Node tar_Node)
	{
		int nodeNumber = graph.getNodeNumber();
		std::priority_queue<ipair, std::vector<ipair>, std::greater<ipair>> pq;
		std::map<int, double> dist;
		int idMax = graph.getMaxNodeId();
		int n = graph.v + 2; 
		parent = new int[n];
		std::map<int, Node> parents; 
		std::bitset<100000> sptBitSet;
		bool check = false; 
	/*	boost::dynamic_bitset<> sptBitSet(idMax + 10); */

		//Init dist
		for (int i = 0; i < nodeNumber; i++)
		{
			int id = graph.Nodes[i]->getId();
			dist[id] = INF;
			parent[i] = -1;
		}
		
		if (src_Node.getId() >= graph.v)
		{
			parent[src_Node.id] = -1; 
			double startLat = src_Node.lat;
			double startLong = src_Node.lon;
			sptBitSet[src_Node.id].flip(); 
			for (int i = 0; i < src_Node.adjacentNodes.size(); i++)
			{
				int idNode = src_Node.adjacentNodes[i]->getId();
				Node* node = graph.getNode(idNode);
				double endLat = node->lat;
				double endLong = node->lon;
				double distance = getDistanceHaversin(startLat, startLong, endLat, endLong);
				if (!sptBitSet[idNode] && (dist[idNode] > dist[src_Node.id] + distance))
				{
					parent[idNode] = src_Node.id;
					dist[idNode] = dist[src_Node.id] + distance;
					sptBitSet[idNode].flip();
					pq.push(std::make_pair(dist[idNode], idNode));
				}
			}
		}
		else
		{
			dist[src_Node.getId()] = 0;
			pq.push(std::make_pair(0, src_Node.getId()));
		}

		if (!graph.isInGraph(tar_Node))
		{
			check = true; 
		}

		while (!pq.empty())
		{
			int idCurrentNode = pq.top().second;
			pq.pop();
			Node* currentNode = graph.getNode(idCurrentNode);
			double startLat = currentNode->lat;
			double startLong = currentNode->lon;
			sptBitSet[idCurrentNode].flip();
			for (int i = 0; i < currentNode->adjacentNodes.size() ; i++)
			{
				int idNode = currentNode->adjacentNodes[i]->getId();
				Node* node = graph.getNode(idNode); 
				double endLat = node->lat;
				double endLong = node->lon;
				double distance = getDistanceHaversin(startLat, startLong, endLat, endLong);
				if (!sptBitSet[idNode] && (dist[idNode] > dist[idCurrentNode] + distance))
				{
					parent[idNode] = idCurrentNode;
					dist[idNode] = dist[idCurrentNode] + distance;
					sptBitSet[idNode].flip(); 
					pq.push(std::make_pair(dist[idNode], idNode));
				}
				if (!check)
				{
					if (idCurrentNode == tar_Node.getId())
						break;
				}
				else
				{

				}
			}
		}
		//printSolution(dist, idSrcNode, idTargetNode, parent);
	}

	void Dijkstra_priority_queue::printSolution(std::map<int, double> dist, int idSrcNode, int idTargetNode, int* parent)
	{
		std::map<int, double>::iterator it;
		for (it = dist.begin(); it != dist.end(); it++)
		{
			int id = it->first;
			double dis = it->second;
			if (dis != INF && id == idTargetNode)
			{
				std::cout << "Distance from: " << idSrcNode << " To: " << id << " is: " << dis << std::endl;
				std::cout << idSrcNode << " ";
				printPath(parent, id);
			}
		}
		//delete[] parent;
	}

	void Dijkstra_priority_queue::printPath(int* parent, int j)
	{
		if (parent[j] == -1)
			return;
		printPath(parent, parent[j]);
		std::cout << j << " ";
	}
	int* Dijkstra_priority_queue::getPath()
	{
		return parent; 
	}
	double Dijkstra_priority_queue::getDistanceHaversin(double startLat, double startLong, double endLat, double endLong)
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
}