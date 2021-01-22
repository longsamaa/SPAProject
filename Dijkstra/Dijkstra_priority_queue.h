#pragma once
#include <iostream>
#include <bitset>
#include <map>
#include <float.h>
//#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include "Graph.h"
namespace Long
{
	class Dijkstra_priority_queue
	{
	public:
		void shortestPath(Long::Graph graph, int idSrcNode, int idTargetNode);
		int* getPath();
	private:
		void printSolution(std::map<int, double> dist, int idSrcNode, int idTargetNode, int* parent);
		void printPath(int* parent, int j);
		double getDistanceHaversin(double startLat, double startLong, double endLat, double endLong); 
	public:
		int* parent; 
	};
}

