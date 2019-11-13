#include "AI.h"

//using std::vector;
using std::cout;
using std::map;
using std::stack;
using std::pair;
using namespace my_std;

static vector<Point> no_vector = {};

namespace AI_N {

	void showfield(Field_size field_Size, vector<vector<unsigned>>& field, vector<Point> &pointsOfInterest= no_vector) {
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
					pointsOfInterest.push_back({ i,j });
					std::cout << "*";
					break;
				case ai_seen:
					std::cout << "o";
					break;
				case notexist:
					std::cout << "x";
					break;
				case rs:
					std::cout << "s";
					break;
				case rc:
					std::cout << "c";
					break;
				}
			}
			std::cout << std::endl;
		}

	}

	stack<Vertex> _stack;

	void AI::dfs(ED_N::environmentDescriptor* environment, vector<vector<unsigned>>& field, managementComponent* component) {
		std::cout << "\n";
		//showfield(environment->getSize(), field);
		std::cout << "\n";

		Vertex vertex = { component->getComp(0)->getCoord().x, component->getComp(0)->getCoord().y , true };

		int x = component->getComp(0)->getCoord().x;
		int y = component->getComp(0)->getCoord().y;

		//coord_Set
		field[component->getCoord().x][component->getCoord().y] = rc;
		field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;


		if (!visited_vertex[vertex.x][vertex.y]) {
			visited_vertex[vertex.x][vertex.y] = true;
			for (size_t i = 0; i < 4; i++)
			{
				component->getInfo(0, environment, field).pointsOfInterest;

				vector<Modules_N::Module*>::iterator it = component->getComp(0)->getModules()->begin();
				while (it != component->getComp(0)->getModules()->end())
				{
					if ((*it)->iAm() == sensor_Module)
						dynamic_cast<sensorModule*>(*it)->turn();
					++it;
				}
			}

			vector<Vertex> newVertex;
			if (field[x - 1][y] == ai_seen)
				newVertex.push_back({ x - 1, y, false });
			if (field[x][y - 1] == ai_seen)
				newVertex.push_back({ x, y - 1, false });
			if (field[x][y + 1] == ai_seen)
				newVertex.push_back({ x, y + 1, false });
			if (field[x + 1][y] == ai_seen)
				newVertex.push_back({ x + 1, y, false });

			if (newVertex.size() == 0) {
				if (_stack.top().x > x) {
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
					component->moveRobot(0, left);
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
				}
				else if (_stack.top().x < x) {
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
					component->moveRobot(0, right);
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
				}
				else if (_stack.top().y > y) {
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
					component->moveRobot(0, up);
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
				}
				else if (_stack.top().y < y) {
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
					component->moveRobot(0, down);
					field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
				}
			}
			_graph.insert(pair<Vertex, vector<Vertex>>(vertex, newVertex));

			_stack.push(vertex);
		}
		else
			return;

		vector<Vertex>::iterator iter;
		iter = _graph[vertex].begin();
		for (iter; iter != _graph[vertex].end(); iter++)
		{

			if ((*iter).x < x) {
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
				component->moveRobot(0, left);
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
			}
			else if ((*iter).x > x) {
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
				component->moveRobot(0, right);
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
			}
			else if ((*iter).y < y) {
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
				component->moveRobot(0, up);
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
			}
			else if ((*iter).y > y) {
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = ai_seen;
				component->moveRobot(0, down);
				field[component->getComp(0)->getCoord().x][component->getComp(0)->getCoord().y] = rs;
			}

			dfs(environment, field, component);
		}


		if (_stack.size() == 0)
			return;
		_stack.pop();

		return;

	}

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

		std::cout << "\n Real Map:\n\n";
		environment->drawJustMap();

		for (int i = 0; i < field_Size.n; ++i) {
			field.push_back(vector<unsigned>());
			for (int j = 0; j < field_Size.m; ++j) {
				field[i].push_back(notexist);
			}
		}

		for (int i = 0; i < field_Size.n; ++i) {
			visited_vertex.push_back(vector<bool>());
			for (int j = 0; j < field_Size.m; ++j) {
				visited_vertex[i].push_back(0);
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////
		//SCAN MANAGEMENT COMPONENTS
		///////////////////////////////////////////////////////////////

		vector<Components_N::managementComponent*>::iterator fmc_it;


		fmc_it = components.begin();
		while (fmc_it != components.end())
		{
			field[(*fmc_it)->getCoord().x][(*fmc_it)->getCoord().y] = rc;
			for (size_t i = 0; i < (*fmc_it)->getNComp()->size(); i++)
			{
				switch ((*fmc_it)->getComp(i)->iAm()) {
				case robot_commander:
				case observe_center:
					field[(*fmc_it)->getComp(i)->getCoord().x][(*fmc_it)->getComp(i)->getCoord().y] = oc;
					break;
				case robot_scout:
					field[(*fmc_it)->getComp(i)->getCoord().x][(*fmc_it)->getComp(i)->getCoord().y] = rs;
					break;
				}
			}
			++fmc_it;
		}

		std::cout << "\n Map created by robotic complex after scan:\n\n";
		showfield(field_Size, field, pointsOfInterest);

		vector<Components_N::managementComponent*>::iterator mc_it;


		mc_it = components.begin();
		while (mc_it != components.end())
		{
			for (size_t i = 0; i < (*mc_it)->getNComp()->size(); i++)
			{
				this->dfs(environment, field, (*mc_it));

				//vector<Point> new_poi = (*mc_it)->getInfo(i, environment, field).pointsOfInterest;
				//vector<Point>::iterator iter = new_poi.begin();
				//while (iter != new_poi.end())
				//{
				//	if (std::find(pointsOfInterest.begin(), pointsOfInterest.end(), *iter) == pointsOfInterest.end()) {
				//		pointsOfInterest.push_back(*iter);
				//	}
				//	++iter;
				//}
			}

			++mc_it;
		}
		



		//vector<Components_N::managementComponent*>::iterator mc_it;

		//
		//mc_it = components.begin();
		//while (mc_it != components.end())
		//{
		//	for (size_t i = 0; i < (*mc_it)->getNComp()->size(); i++)
		//	{
		//		vector<Point> new_poi = (*mc_it)->getInfo(i, environment, field).pointsOfInterest;

		//		vector<Point>::iterator iter = new_poi.begin();
		//		while (iter != new_poi.end())
		//		{
		//			if (std::find(pointsOfInterest.begin(), pointsOfInterest.end(), *iter) == pointsOfInterest.end()) {
		//				pointsOfInterest.push_back(*iter);
		//			}
		//			++iter;
		//		}
		//	}

		//	++mc_it;
		//}


		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		std::cout << "\n Map created by robotic complex after scan:\n\n";
		showfield(field_Size, field, pointsOfInterest);

		return pointsOfInterest;
	}
};