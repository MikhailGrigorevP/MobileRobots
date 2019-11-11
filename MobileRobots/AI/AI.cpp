#include "AI.h"
#include <list>
using namespace std;

class Graph {
	int V; 
	list<int>* adj; 
	bool* visited; 
public:
	Graph(int V); 
	void addEdge(int v, int w); 
	void DFS(int v); 
};
Graph::Graph(int V) {
	this->V = V;
	adj = new list<int>[V];
	this->visited = new bool[this->V];
	for (int i = 0; i < this->V; i++) this->visited[i] = false;
}
void Graph::addEdge(int v, int w) {
	adj[v].push_back(w); 
	
}

void Graph::DFS(int v) {
	this->visited[v] = true;
	cout << v << " ";

	list<int>::iterator it;
	for (it = adj[v].begin(); it != adj[v].end(); ++it) {
		if (!this->visited[*it]) {
			DFS(*it);
		}
	}
}

namespace AI_N {

	//! Method that returns all points of intererst
	vector<Point> AI::findInterestPoints(ED_N::environmentDescriptor* environment) {

		///////////////////////////////////////////////////////////////
		//INIT FIELD
		///////////////////////////////////////////////////////////////
		vector<Components_N::managementComponent*> components;
		environment->getAllManagementComponents(components);

		vector<Point> pointsOfInterest;
		vector<vector<unsigned>> field;
		Field_size field_Size = environment->getSize();

		int V = field_Size.m * field_Size.n;
		Graph G(V);


		for (int i = 0; i < field_Size.n; ++i) {
			field.push_back(vector<unsigned>());
			for (int j = 0; j < field_Size.m; ++j) {
				field[i].push_back(none_cell);
			}
		}
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////
		//SCAN MANAGEMENT COMPONENTS
		///////////////////////////////////////////////////////////////

		vector<Components_N::managementComponent*>::iterator mc_it;

		
		mc_it = components.begin();
		while (mc_it != components.end())
		{
			for (size_t i = 0; i < (*mc_it)->getNComp()->size(); i++)
			{
				vector<Point> new_poi = (*mc_it)->getInfo(i, environment, field).pointsOfInterest;

				vector<Point>::iterator iter = new_poi.begin();
				while (iter != new_poi.end())
				{
					if (std::find(pointsOfInterest.begin(), pointsOfInterest.end(), *iter) == pointsOfInterest.end()) {
						pointsOfInterest.push_back(*iter);
					}
					++iter;
				}
			}

			++mc_it;
		}

		G.addEdge(0, 1);
		G.addEdge(0, 2);
		G.addEdge(1, 2);
		G.addEdge(2, 0);
		G.addEdge(2, 3);
		G.addEdge(3, 3);

		//G.DFS(src);

		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		if (field_Size.m == 0 || field_Size.n == 0)
			throw std::exception(">>> Environment is empty");
		for (int j = 0; j < field_Size.m; ++j) {
			for (int i = 0; i < field_Size.n; ++i) {
				switch (field[i][j]) {
				case none_cell:
					std::cout << "~";
					break;
				case barrier:
					std::cout << "#";
					break;
				case interest_point:
					std::cout << "*";
					break;
				case ai_seen:
					std::cout << "o";
					break;
				}
			}
			std::cout << std::endl;
		}

		return pointsOfInterest;
	}
};