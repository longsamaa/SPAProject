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
			this->id = _node.id;
			this->lon = _node.lon;
			this->lat = _node.lat;
			for (int i = 0; i < _node.adjacentNodes.size(); i++)
			{
				adjacentNodes.push_back(_node.adjacentNodes.at(i));
			}
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
		bool compareAdjacentNodes(Long::Node _node); 
	public:
		int id; 
		double lon; 
		double lat; 
		std::vector<Node*> adjacentNodes; 
	};
}

