#include "AI.h"
#include <chrono>
//using std::vector;
using std::cout;
using std::map;
using std::stack;
using std::pair;
using namespace my_std;

static bool IS_ACTIVE = true;

static vector<Point> no_vector = {};

namespace AI_N {
	void showfield(Field_size field_Size, vector<vector<unsigned>>& field, vector<Point>& pointsOfInterest = no_vector) {
		if (field_Size.m == 0 || field_Size.n == 0)
			throw std::exception(">>> Environment is empty");
		for (int j = 0; j < field_Size.m; ++j) {
			for (int i = 0; i < field_Size.n; ++i) {
				switch (field[i][j]) {
				case none_cell:
					std::cout << "x";
					break;
				case barrier:
					std::cout << "#";
					break;
				case interest_point:
					pointsOfInterest.push_back({ i,j });
					std::cout << "*";
					break;
				case ai_seen:
					if (((i > 0) && (field[i - 1][j] != notexist)) && ((j > 0) && (field[i][j - 1] != notexist)) && ((j < field_Size.m - 1) && (field[i][j + 1] != notexist)) && ((i < field_Size.n - 1) && (field[i + 1][j] != notexist)))
						std::cout << (unsigned char)(206);
					else if (((j > 0) && (field[i][j - 1] != notexist)) && ((j < field_Size.m - 1) && (field[i][j + 1] != notexist)) && ((i < field_Size.n - 1) && (field[i + 1][j] != notexist)))
						std::cout << (unsigned char)(204);
					else if (((i > 0) && (field[i - 1][j] != notexist)) && ((j > 0) && (field[i][j - 1] != notexist)) && ((j < field_Size.m - 1) && (field[i][j + 1] != notexist)))
						std::cout << (unsigned char)(185);
					else if (((i > 0) && (field[i - 1][j] != notexist)) && ((j < field_Size.m - 1) && (field[i][j + 1] != notexist)) && ((i < field_Size.n - 1) && (field[i + 1][j] != notexist)))
						std::cout << (unsigned char)(203);
					else if (((i > 0) && (field[i - 1][j] != notexist)) && ((j > 0) && (field[i][j - 1] != notexist)) && ((i < field_Size.n - 1) && (field[i + 1][j] != notexist)))
						std::cout << (unsigned char)(202);
					else if (((i > 0) && (field[i - 1][j] != notexist)) && ((j < field_Size.m - 1) && (field[i][j + 1] != notexist)))
						std::cout << (unsigned char)(187);
					else if (((j > 0) && (field[i][j - 1] != notexist)) && ((i < field_Size.n - 1) && (field[i + 1][j] != notexist)))
						std::cout << (unsigned char)(200);
					else if (((i > 0) && (field[i - 1][j] != notexist)) && ((j > 0) && (field[i][j - 1] != notexist)))
						std::cout << (unsigned char)(188);
					else if (((j < field_Size.m - 1) && (field[i][j + 1] != notexist)) && ((i < field_Size.n - 1) && (field[i + 1][j] != notexist)))
						std::cout << (unsigned char)(201);
					else if (((i > 0) && (field[i - 1][j] != notexist)) || ((i < field_Size.n - 1) && (field[i + 1][j] != notexist)))
						std::cout << (unsigned char)(205);
					else
						std::cout << (unsigned char)(186);
					break;
				case notexist:
					std::cout << " ";
					break;
				case rs:
					std::cout << "s";
					break;
				case rsd:
					std::cout << "$";
					break;
				case rc:
					std::cout << "c";
					break;
				case oc:
					std::cout << "o";
					break;
				case cc:
					std::cout << "g";
					break;
				}
			}
			std::cout << std::endl;
		}

	}

	void showfield(Field_size field_Size, vector<vector<int>>& field, vector<Point>& pointsOfInterest = no_vector) {
		if (!IS_ACTIVE) {
			if (field_Size.m == 0 || field_Size.n == 0)
				throw std::exception(">>> Environment is empty");
			for (int j = 0; j < field_Size.m; ++j) {
				for (int i = 0; i < field_Size.n; ++i) {
					switch (field[i][j]) {
					case V_not:
						std::cout << "0";
						break;
					case V_partly:
						std::cout << "1";
						break;
					case V_visited:
						std::cout << "2";
						break;
					}
				}
				std::cout << std::endl;
			}
		}

	}

	void show(ED_N::environmentDescriptor* environment, int x, int y, managementComponent* component, vector<vector<unsigned>>& field) {
		if (IS_ACTIVE) {
			if (system("CLS"))
				system("clear");
			std::cout << "\n>> rc " << component->getCoord().x << " " << component->getCoord().y << std::endl;
			std::cout << "\n>> rs " << x << " " << y << std::endl;
			showfield(environment->getSize(), field);
			std::cout << "\n";
		}
	}


	std::stack<Point> way_back;

	void AI::bfs(Point v, Point e, vector<vector<unsigned>>& field) {

		queue<Point> q;
		Point s = v;
		vector<Point> p;
		vector<vector<int>> mark;
		std::map<Point, vector<Vertex>> _n_graph;

		for (int j = 0; j < field.size(); ++j) {

			mark.push_back(vector<int>());
			for (int i = 0; i < field[j].size(); ++i) {

				mark[j].push_back(0);
				Point potential_end;
				if (field[i][j] != notexist) {

					vector<Vertex> nVertex;
					potential_end = { i - 1, j };
					if ((i > 0 && (field[i - 1][j] == ai_seen)) || (potential_end == e))
						nVertex.push_back({ i - 1, j });

					potential_end = { i, j - 1 };
					if ((j > 0 && (field[i][j - 1] == ai_seen)) || (potential_end == e))
						nVertex.push_back({ i, j - 1 });

					potential_end = { i, j + 1 };
					if ((j < field.size() - 1 && (field[i][j + 1] == ai_seen)) || (potential_end == e))
						nVertex.push_back({ i, j + 1 });

					potential_end = { i + 1, j };
					if ((i < field[j].size() - 1 && (field[i + 1][j] == ai_seen)) || (potential_end == e))
						nVertex.push_back({ i + 1, j });

					_n_graph.insert(pair<Point, vector<Vertex>>({ i, j }, nVertex));
				}
			}
		}

		for (size_t j = 0; j < field.size() * field.size(); j++)
		{
			p.push_back({ 0,0 });
		}

		q.push(v);
		mark[v.x][v.y] = true;

		while (!q.empty()) {
			v = q.front();
			q.pop();
			if (v == e)
				break;
			//Vertex
			vector<Vertex>::iterator iter;
			iter = _n_graph[v].begin();
			for (iter; iter != _n_graph[v].end(); iter++) {
				if (s.x == 3 && s.y == 6) {
				}
				if (mark[(*iter).x][(*iter).y]) {
					continue;
				}
				q.push({ (*iter).x,(*iter).y });
				mark[(*iter).x][(*iter).y] = true;
				int num = (*iter).y * field.size() + (*iter).x;
				p[num] = v;
			}
		}

		if (!mark[e.x][e.y])
			return;
		else {
			int num = e.y * field.size() + e.x;
			int end = s.y * field.size() + s.x;
			while (num != end) {
				way_back.push(p[num]);
				num = p[num].y * field.size() + p[num].x;
			}
			way_back.pop();
		};
		return;
	}

	vector<stack<Point>> _stacks;
	void AI::dfs(ED_N::environmentDescriptor* environment, vector<vector<unsigned>>& field, managementComponent* component) {

		vector<Component*> dead_robots;
		vector<stack<Point>> _dead_stacks;

		vector<Component*> UselessObserveCenters;

		bool scanning;

		bool last_chance = false;

		do {

			scanning = false;

			int x2 = component->getCoord().x;
			int y2 = component->getCoord().y;
			int radius = 0;
			vector<Modules_N::Module*>::iterator it = component->getModules()->begin();
			while (it != component->getModules()->end())
			{
				if ((*it)->iAm() == management_Module) {
					radius = dynamic_cast<managementModule*>(*it)->getR();
					break;
				}
				it++;
			}

			int save = -1;

			for (size_t j = 0; j < component->getNComp()->size(); j++)
			{
				int x = component->getComp(j)->getCoord().x;
				int y = component->getComp(j)->getCoord().y;
				Point nopoint = { -1, -1 };
				if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius) && (save == -1)) {
					save = j;
				}
				else if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius) && (save >= 0)) {
					vector<Modules_N::Module*>::iterator it = component->getModules()->begin();
					while (it != component->getModules()->end())
					{
						if ((*it)->iAm() == management_Module) {
							dead_robots.push_back(component->getComp(j));
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rsd;
							dynamic_cast<managementModule*>(*it)->freeResourse(component, j);
							_dead_stacks.push_back(_stacks[j]);
							_dead_stacks[0].push({ x,y });
							_stacks.erase(j);
							--j;
							break;
						}
						it++;
					}
				}

				//GIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIVE A CHANCE 
				else if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius) && (save >= 0)) {

					if ((!_stacks[j].empty()) && (_stacks[j].top() != nopoint)) {
						vector<Modules_N::Module*>::iterator it = component->getModules()->begin();
						while (it != component->getModules()->end()) {
							if ((*it)->iAm() == management_Module) {
								dead_robots.push_back(component->getComp(save));
								field[component->getComp(save)->getCoord().x][component->getComp(save)->getCoord().y] = rsd;
								dynamic_cast<managementModule*>(*it)->freeResourse(component, save);
								_dead_stacks.push_back(_stacks[save]);

								_dead_stacks[0].push({ x,y });
								_stacks.erase(save);
								save = -2;
								--j;
								break;
							}
							++it;
						}
					}
					else {
						vector<Modules_N::Module*>::iterator it = component->getModules()->begin();
						while (it != component->getModules()->end()) {
							if ((*it)->iAm() == management_Module) {
								dead_robots.push_back(component->getComp(save));
								field[component->getComp(save)->getCoord().x][component->getComp(save)->getCoord().y] = rsd;
								dynamic_cast<managementModule*>(*it)->freeResourse(component, save);
								_dead_stacks.push_back(_stacks[save]);

								_dead_stacks[0].push({ x,y });
								_stacks.erase(save);
								save = -2;
								--j;
								break;
							}
							++it;
						}
					}
				}

			}

			if (save >= 0) {

				int x = component->getComp(save)->getCoord().x;
				int y = component->getComp(save)->getCoord().y;

				while (sqrt((x - component->getCoord().x) * (x - component->getCoord().x) + (y - component->getCoord().y) * (y - component->getCoord().y)) > 1) {

					show(environment, x, y, component, field);

					showfield(environment->getSize(), visited_vertex);

					if (way_back.size() != 0) {

						int x2 = component->getCoord().x;
						int y2 = component->getCoord().y;

						if (way_back.top().x > x2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						else if (way_back.top().x < x2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						else if (way_back.top().y > y2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						else if (way_back.top().y < y2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(up);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						way_back.pop();

						x2 = component->getCoord().x;
						y2 = component->getCoord().y;

						continue;
					}

					else {
						this->bfs({ x2, y2 }, { x,y }, field);
					}

				}

			}

			//SCANNING DEAD
			for (size_t i = 0; i < dead_robots.size(); i++)
			{
				if (component->getNumD() <= component->getNum())
					break;
				int x = dead_robots[i]->getCoord().x;
				int y = dead_robots[i]->getCoord().y;
				if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius)
					break;
				if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius)
					while (it != component->getModules()->end())
					{
						if ((*it)->iAm() == management_Module) {
							dynamic_cast<managementModule*>(*it)->sendResourse(component, dead_robots[i]);
							_stacks.push_back(_dead_stacks[i]);
							field[dead_robots[i]->getCoord().x][dead_robots[i]->getCoord().y] = rs;
							_dead_stacks.erase(i);
							dead_robots.erase(i);
							--i;
							break;
						}
						it++;
					}
			}

			//SCANNING OC
			for (size_t i = 0; i < UselessObserveCenters.size(); i++)
			{
				if (component->getNumD() <= component->getNComp()->size())
					break;

				int x = UselessObserveCenters[i]->getCoord().x;
				int y = UselessObserveCenters[i]->getCoord().y;
				if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius)
					break;
				if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius) {
					while (it != component->getModules()->end())
					{
						if ((*it)->iAm() == management_Module) {
							dynamic_cast<managementModule*>(*it)->sendResourse(component, UselessObserveCenters[i]);
							component->getInfo(component->getNComp()->size() - 1, environment, field);
							dynamic_cast<managementModule*>(*it)->freeResourse(component, component->getNComp()->size() - 1);
							if (UselessObserveCenters.size() > 1)
								UselessObserveCenters.erase(i);
							else
								UselessObserveCenters.clear();
						}
						++it;
					}
				}
			}


			while (!way_back.empty())
				way_back.pop();

			if (last_chance && dead_robots.size() != 0) {

				int x = dead_robots[0]->getCoord().x;
				int y = dead_robots[0]->getCoord().y;


				while (sqrt((x - component->getCoord().x) * (x - component->getCoord().x) + (y - component->getCoord().y) * (y - component->getCoord().y)) > radius) {

					show(environment, x, y, component, field);

					showfield(environment->getSize(), visited_vertex);
					if (way_back.size() != 0) {

						int x2 = component->getCoord().x;
						int y2 = component->getCoord().y;

						if (way_back.top().x > x2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						else if (way_back.top().x < x2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						else if (way_back.top().y > y2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						else if (way_back.top().y < y2) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>(component)->moveRobotInDirection(up);
							field[component->getCoord().x][component->getCoord().y] = rc;
						}
						way_back.pop();

						x2 = component->getCoord().x;
						y2 = component->getCoord().y;

						continue;
					}

					else {
						this->bfs({ x2, y2 }, { x,y }, field);
					}
					last_chance = false;
				}

				it = component->getModules()->begin();
				if (sqrt((x - component->getCoord().x) * (x - component->getCoord().x) + (y - component->getCoord().y) * (y - component->getCoord().y)) <= radius)
					while (it != component->getModules()->end())
					{
						if ((*it)->iAm() == management_Module) {
							dynamic_cast<managementModule*>(*it)->sendResourse(component, dead_robots[0]);
							_stacks.push_back(_dead_stacks[0]);
							field[dead_robots[0]->getCoord().x][dead_robots[0]->getCoord().y] = rs;
							_dead_stacks.erase(0);
							dead_robots.erase(0);
							break;
						}
						it++;
					}
			}

			//COMPOENTS
			for (size_t j = 0; j < component->getNComp()->size(); j++)
			{


				Point nopoint = { -1,-1 };
				if ((_stacks[j].size()) && (_stacks[j].top() == nopoint))
					continue;

				Point vertex = { component->getComp(j)->getCoord().x, component->getComp(j)->getCoord().y };

				int x = component->getComp(j)->getCoord().x;
				int y = component->getComp(j)->getCoord().y;
				//Coord_Set
				field[component->getCoord().x][component->getCoord().y] = rc;
				field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;

				//SCAN VERTEX
				if (visited_vertex[vertex.x][vertex.y] == V_not) {
					visited_vertex[vertex.x][vertex.y] = V_partly;

					vector<Component*> diffComponents;
					//GET ALL INFO FROM ROBOT
					for (size_t i = 0; i < 4; i++)
					{
						diffComponents = component->getInfo(j, environment, field).components;
						
						vector<Component*>::iterator it0 = diffComponents.begin();
						while (it0 != diffComponents.end())
						{
							if (((*it0)->iAm() == observe_center)&& (visited_vertex[(*it0)->getCoord().x][(*it0)->getCoord().y]!=V_visited) ) {
								UselessObserveCenters.push_back(*it0);
								visited_vertex[(*it0)->getCoord().x][(*it0)->getCoord().y] = V_visited;
								field[(*it0)->getCoord().x][(*it0)->getCoord().y] = oc;
							}
							++it0;
						}

						vector<Modules_N::Module*>::iterator it = component->getComp(j)->getModules()->begin();
						while (it != component->getComp(j)->getModules()->end())
						{
							if ((*it)->iAm() == sensor_Module)
								dynamic_cast<sensorModule*>(*it)->turn();
							++it;
						}
					}

					//CHECK CELLS AROUND THE ROBOT
					vector<Vertex> newVertex;
					Point test_point;
					test_point = { x - 1,y };
					if ((y > 0) && (field[x - 1][y] == ai_seen) && visited_vertex[x - 1][y] == V_not)
						newVertex.push_back({ x - 1, y });
					test_point = { x,y - 1 };
					if ((y > 0) && (field[x][y - 1] == ai_seen) && visited_vertex[x][y - 1] == V_not)
						newVertex.push_back({ x, y - 1 });
					test_point = { x,y + 1 };
					if ((y < environment->getSize().m - 1) && (field[x][y + 1] == ai_seen) && visited_vertex[x][y + 1] == V_not)
						newVertex.push_back({ x, y + 1 });
					test_point = { x + 1,y };
					if ((x < environment->getSize().n - 1) && (field[x + 1][y] == ai_seen) && visited_vertex[x + 1][y] == V_not)
						newVertex.push_back({ x + 1, y });

					//IF NO NEW WAYS -> V == visited
					if (newVertex.size() == 0) {

						visited_vertex[vertex.x][vertex.y] = V_visited;
					}

					_graph.insert(pair<Point, vector<Vertex>>(vertex, newVertex));
					_stacks[j].push(vertex);
				}

				//SCAN WAYS
				if (visited_vertex[vertex.x][vertex.y] == V_partly) {
					if (_stacks[j].top() != vertex)
						_stacks[j].push(vertex);
					vector<Vertex>::iterator iter;
					iter = _graph[vertex].begin();
					int ways_left = false;
					for (iter; iter != _graph[vertex].end(); iter++)
					{
						if ((*iter).visited == V_not) {
							ways_left = true;
							(*iter).visited = V_visited;


							if ((*iter).x < x) {
								if (field[(*iter).x][(*iter).y] == rc) {
									if ((*iter).x > 0 && (field[(*iter).x - 1][(*iter).y] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).y < environment->getSize().m - 1 && (field[(*iter).x][(*iter).y + 1] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).y > 0 && (field[(*iter).x][(*iter).y - 1] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(up);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
								}
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
								component->moveRobot(j, left);
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
							}
							else if ((*iter).x > x) {
								if (field[(*iter).x][(*iter).y] == rc) {
									if ((*iter).x < environment->getSize().n - 1 && (field[(*iter).x + 1][(*iter).y] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).y < environment->getSize().m - 1 && (field[(*iter).x][(*iter).y + 1] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).y > 0 && (field[(*iter).x][(*iter).y - 1] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(up);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
								}
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
								component->moveRobot(j, right);
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
							}
							else if ((*iter).y < y) {

								if (field[(*iter).x][(*iter).y] == rc) {
									if ((*iter).x > 0 && (field[(*iter).x - 1][(*iter).y] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).x < environment->getSize().n - 1 && (field[(*iter).x + 1][(*iter).y] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).y > 0 && (field[(*iter).x][(*iter).y - 1] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(up);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
								}
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
								component->moveRobot(j, up);
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
							}
							else if ((*iter).y > y) {
								if (field[(*iter).x][(*iter).y] == rc) {
									if ((*iter).x > 0 && (field[(*iter).x - 1][(*iter).y] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).x < environment->getSize().n - 1 && (field[(*iter).x + 1][(*iter).y] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if ((*iter).y < environment->getSize().m - 1 && (field[(*iter).x][(*iter).y + 1] == ai_seen)) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
								}
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
								component->moveRobot(j, down);
								field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
							}
							break;
						}
					}
					if (!ways_left)
						visited_vertex[vertex.x][vertex.y] = V_visited;
				}
				if (visited_vertex[vertex.x][vertex.y] == V_visited)
					_stacks[j].pop();

				if (_stacks[j].size() != 0) {
					if (_stacks[j].top() == nopoint) {
						last_chance = false;
						break;
					}

					scanning = true;
					if (visited_vertex[vertex.x][vertex.y] == V_visited) {
						if ((_stacks[j].top().x > x) && (field[x + 1][y] != notexist)) {
							if (field[_stacks[j].top().x][_stacks[j].top().y] == rc) {
								if (field[_stacks[j].top().x][_stacks[j].top().y] == rc) {
									if (field[_stacks[j].top().x + 1][_stacks[j].top().y] == ai_seen) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if (field[_stacks[j].top().x][_stacks[j].top().y + 1] == ai_seen) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
									else if (field[_stacks[j].top().x][_stacks[j].top().y - 1] == ai_seen) {
										field[component->getCoord().x][component->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>(component)->moveRobotInDirection(up);
										field[component->getCoord().x][component->getCoord().y] = rc;
									}
								}
							}
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
							component->moveRobot(j, right);
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
						}
						else if ((_stacks[j].top().x < x) && (field[x - 1][y] != notexist)) {
							if (field[_stacks[j].top().x][_stacks[j].top().y] == rc) {
								if (field[_stacks[j].top().x - 1][_stacks[j].top().y] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
								else if (field[_stacks[j].top().x][_stacks[j].top().y + 1] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
								else if (field[_stacks[j].top().x][_stacks[j].top().y - 1] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(up);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
							}
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
							component->moveRobot(j, left);
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
						}
						else if ((_stacks[j].top().y > y) && (field[x][y + 1] != notexist)) {
							if (field[_stacks[j].top().x][_stacks[j].top().y] == rc) {
								if (field[_stacks[j].top().x - 1][_stacks[j].top().y] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
								else if (field[_stacks[j].top().x][_stacks[j].top().y + 1] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(down);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
								else if (field[_stacks[j].top().x + 1][_stacks[j].top().y] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
							}
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
							component->moveRobot(j, down);
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
						}
						else if ((_stacks[j].top().y < y) && (field[x][y - 1] != notexist)) {
							if (field[_stacks[j].top().x][_stacks[j].top().y] == rc) {
								if (field[_stacks[j].top().x - 1][_stacks[j].top().y] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(left);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
								else if (field[_stacks[j].top().x + 1][_stacks[j].top().y] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
								else if (field[_stacks[j].top().x + 1][_stacks[j].top().y] == ai_seen) {
									field[component->getCoord().x][component->getCoord().y] = ai_seen;
									dynamic_cast<robotCommander*>(component)->moveRobotInDirection(right);
									field[component->getCoord().x][component->getCoord().y] = rc;
								}
							}
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = ai_seen;
							component->moveRobot(j, up);
							field[component->getComp(j)->getCoord().x][component->getComp(j)->getCoord().y] = rs;
						}
						//	std::cout << "\nHERE\n";
					}
				}
				else if (dead_robots.size() != 0) {
					scanning = true;
					last_chance = true;
				}

				show(environment, x, y, component, field);
				showfield(environment->getSize(), visited_vertex);

				if (_stacks[j].size() == 0) {
					_stacks[j].push({ -1, -1 });
				}
			}

		} while (scanning);
		return;

	}

	//! Method that returns all points of intererst
	vector<Point> AI::findInterestPoints(ED_N::environmentDescriptor* environment) {

		///////////////////////////////////////////////////////////////
		///INIT FIELD
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
			visited_vertex.push_back(vector<int>());
			for (int j = 0; j < field_Size.m; ++j) {
				visited_vertex[i].push_back(0);
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////
		///SCAN MANAGEMENT COMPONENTS
		///////////////////////////////////////////////////////////////

		vector<Components_N::managementComponent*>::iterator fmc_it;


		fmc_it = components.begin();
		while (fmc_it != components.end())
		{
			field[(*fmc_it)->getCoord().x][(*fmc_it)->getCoord().y] = rc;
			for (size_t i = 0; i < (*fmc_it)->getNComp()->size(); i++)
			{
				_stacks.push_back(stack<Point>());
				switch ((*fmc_it)->getComp(i)->iAm()) {
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

		std::cout << "\n Map created by robotic complex before scan:\n\n";
		showfield(field_Size, field, pointsOfInterest);

		vector<Components_N::managementComponent*>::iterator mc_it;


		mc_it = components.begin();
		while (mc_it != components.end())
		{

			this->dfs(environment, field, (*mc_it));

			++mc_it;
		}


		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		std::cout << "\n Map created by robotic complex after scan:\n\n";
		showfield(field_Size, field, pointsOfInterest);

		return pointsOfInterest;
	}
};