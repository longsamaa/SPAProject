#pragma once
#include <vector>
namespace Long {
	class Node
	{
	public: 
		Node() { 
			int id = 0; 
			double lat = -181; 
			double lon = -91; 
			adjacentNodes.clear(); 
		}
		Node(int Id, double Lon, double Lat)
		{
			id = Id; 
			lon = Lon; 
			lat = Lat;
		}
		Node(const Node& _node)
		{
			id = _node.id; 
			lon = _node.lon; 
			lat = _node.lat; 
			adjacentNodes = _node.adjacentNodes; 
		}
		inline int getId()
		{
			return id; 
		}
		void addAdjacentNode(Node* node);
		void deleteAdjacentNode(Node* node); 
		bool isNode()
		{
			return lat != -181 && lon != -91; 
		}
		void setNull()
		{
			lat = -181; 
			lon = -91; 
			adjacentNodes.clear(); 
		}
		void copy(Node _node);
		void copy_P(Node& _node); 
		void set(int _id, double _lat, double _lon)
		{
			id = _id; 
			lat = _lat; 
			lon = _lon; 
			adjacentNodes.clear(); 
		}
	public:
		int id; 
		double lon; 
		double lat; 
		std::vector<Node*> adjacentNodes; 
	};
}

