#pragma once
#include <vector>
#include <map>
#include <queue>
#include <cmath>
#include "Node.h"
#include "Edge.h"
typedef std::pair<double, int> ipair;
namespace Long
{
	class Graph
	{
	public: 
		Graph() {
			v = 0;
			e = 0;
			top = 0; 
			left = 0; 
			right = 0; 
			bottom = 0; 
			src = 0; 
			target = 0; 
		}
		void setVE(int V, int E) {
			v = V;
			e = E;
		}
		inline int getNodeNumber() {
			return Nodes.size();
		}
		void addEdgeQq(int u, int v, double w);
		void addNode(Long::Node* node); 
		void addEdge(Long::Edge* edge); 
		void addAdjacentNode(Node* rootNode, Node* adadjacentid);
		void deleteNode(Node* node); 
		Node* getNode(int id);
		int getMaxNodeId(); 
		double getDistanceHaversin(double startLat, double startLong, double endLat, double endLong); 
	public: 
		int v; 
		int e; 
		int src; 
		int target;
		double top; 
		double left; 
		double right; 
		double bottom; 
		std::vector<Node*> Nodes; 
		std::vector<Edge*> Edges; 
		std::map<int, std::vector<ipair>> EdgesQq;
	};
}

