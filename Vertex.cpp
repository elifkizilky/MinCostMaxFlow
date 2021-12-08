#include <iostream>
#include <unordered_map>
#include <string>
#include <stack>
#include <limits>

using namespace std;

// I utilized from the code: https://github.com/elifkizilky/cmpe250-fall20/blob/main/PS11/utils.cpp

class Vertex{
public:
int ID;
unordered_map<int, int> adjacencyMap; // first is vertex that vertex goes, second is the cost

Vertex(int ID = 0){
	this->ID = ID;
}

Vertex(const Vertex& vertex){
	this->ID = vertex.ID;
	this->adjacencyMap = vertex.adjacencyMap;

}

void addEdge(int id, int cost) {
		adjacencyMap[id] = cost;
}
};

class AugmentingPath{
public:
	int amount; //meaning capacity
	stack<int> path;

	AugmentingPath(){
		this->amount = -1; // to indicate there is no path
	}
	AugmentingPath(const AugmentingPath& augmentingPath){
		this->amount = augmentingPath.amount;
		this->path = augmentingPath.path;
	}

	AugmentingPath(int amount, stack<int> path) {
		this->amount = amount;
		this->path = path;
	}
};
