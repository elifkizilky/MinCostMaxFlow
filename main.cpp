#include "Vertex.cpp"
#include <queue>
#include <fstream>
using namespace std;

//I utilized the code from: https://github.com/rizaozcelik/cmpe250-fall20/blob/main/PS12/bellman_ford.cpp

stack<int> trace(int vertex, const vector<int>& pre){
	stack<int> cycle;
	int parent = pre[vertex];
	cycle.push(vertex);


	while(vertex != parent){ 
		cycle.push(parent);
	    parent = pre[parent];
	}
	cycle.push(vertex);
	return cycle;
}

//I utilized the code from: https://konaeakira.github.io/assets/code-snippets/cycle-detection-with-spfa.cpp

bool detect_cycle(int numOfVertices, const vector<int>& pre, stack<int>& cycle){ //cycle is the path

	bool visited[numOfVertices + 1], on_stack[numOfVertices + 1];
	std::vector<int> vec;
    std::fill(on_stack, on_stack + 1 +numOfVertices, false);
    std::fill(visited, visited + 1 +numOfVertices, false);
     for (int i = 1; i < numOfVertices + 1; i++){
        if (!visited[i])
        {
            for (int j = i; j != -1; j = pre[j]){
                if (!visited[j])
                {
                    visited[j] = true;
                    vec.push_back(j);
                    on_stack[j] = true;
                }
                 else
                {
                    if (on_stack[j]){
                    	cycle = trace(j,pre);
                        return true;
                    }
                    break;
                }
            }
            for (int j : vec){
                on_stack[j] = false;
            }
            vec.clear();
        }
    }
    return false;
}

// I utilized the code from: https://github.com/elifkizilky/cmpe250-fall20/blob/main/PS11/ford_fulkerson.cpp

void augment(unordered_map<int, Vertex>& residualGraph, AugmentingPath augmentingPath, int& totCost) {
	stack<int> path = augmentingPath.path;

	while (path.size() > 1) {
		int fromVertex = path.top();
		path.pop();
		int toVertex = path.top();
		
		int cost = residualGraph[fromVertex].adjacencyMap[toVertex];
		totCost += cost;
		//since capacity is 1, the edge is deleted and added an opposite edge with negative of the cost of the former edge
		residualGraph[fromVertex].adjacencyMap.erase(toVertex);
		residualGraph[toVertex].adjacencyMap[fromVertex] = (cost*(-1)); 
		residualGraph[toVertex].addEdge(fromVertex, (cost*(-1)));
		
	}
}
	// I utilized the code from: https://konaeakira.github.io/assets/code-snippets/cycle-detection-with-spfa.cpp

	bool findNegativeCycle(unordered_map<int, Vertex>& graph, stack<int>& path, int numOfVertices, int& totCost){
	 vector<int> distances(numOfVertices + 1, 0);
     vector<int> predecessors(numOfVertices + 1, -1);
     bool in_queue[numOfVertices];

     std::fill(in_queue, in_queue + numOfVertices+1, true);
     queue<int> q;

     for (int i = 0; i < numOfVertices; i++){
     	q.push(i+1);
     }

     int iter = 0;
     while (!q.empty()){
     	int u = q.front();
     	in_queue[u] = false;
     	q.pop();
     	for(auto edge : graph[u].adjacencyMap){
     		int v = edge.first;
     		 if(distances[u] + edge.second < distances[v]){
     		 	predecessors[v] = u;
     		 	distances[v] = distances[u] + edge.second;
     		 	iter += 1;
     		 	if(iter == numOfVertices){
     		 		iter = 0;
     		 	if(detect_cycle(numOfVertices, predecessors, path)){
     		 		return true;
     		 	}
     		 }
     		 	if (!in_queue[v])
				{
					q.push(v);
					in_queue[v] = true;
				}
     		 }
     	}
     }
     			if(detect_cycle(numOfVertices, predecessors, path)){ 
     		 		return true;
     		 	}

     return false;
} 

	
int main(int argc, char* argv[]) {
	string infile_name = argv[1];
	string outfile_name = argv[2];

	ifstream infile;
	infile.open(infile_name);

	ofstream outfile;
	outfile.open(outfile_name);

	int numOfCases;
	infile >> numOfCases;
	int numOfVertices = 0;

	int numOfCables;
	
	for(int i = 0; i < numOfCases; i++){
		unordered_map<int, Vertex> graph;
		
		infile >> numOfCables;
		numOfVertices = 2*numOfCables;

		for (int i = 0; i < numOfCables; i++){
			graph[i+1] = Vertex(i+1);
			int voltage; 
			for (int j = 0; j < numOfCables; j++)
			{
				infile >> voltage;
				int cost = (-1 * voltage);
				graph[i+1].addEdge(j + 1 + numOfCables, cost);
			}		
		}
		int maxVoltage = 0;
		int weight;

		for(int m = 0;  m < numOfCables; m++){
			weight = graph[m+1].adjacencyMap[m + 1 + numOfCables];
			maxVoltage += weight;
			graph[m+1].adjacencyMap.erase(m + 1 + numOfCables);
			graph[m+ 1+ numOfCables].adjacencyMap[m+1] = (weight*(-1)); 
		    graph[m+ 1+ numOfCables].addEdge(m+1, (weight*(-1)));
		    

		}
		stack<int> path;


	    while(findNegativeCycle(graph, path, numOfVertices, maxVoltage)){
	    	AugmentingPath augmentingPath(1,path);
     		augment(graph,augmentingPath,maxVoltage);
	    }

		outfile << (maxVoltage*(-1)) << endl;
	
}

    return 0;
}
