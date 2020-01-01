#include "AI.h"
#include <chrono>
#include <algorithm>
#include "..//Render/render.h"
#include "SDL.h"

using std::vector;
using std::cout;
using std::map;
using std::stack;
using std::pair;
//using namespace my_std;
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
static bool IS_ACTIVE = true;
static bool TryClear = false;

static vector<Point> no_vector = {};
int num = 0;
namespace AI_N {
	bool checkMoveAbility(vector<vector<unsigned>>& field, int x, int y, int dist, int vel, bool isIgnoreCommander = false) {
		int steps = 0;
		if (dist == right) {
			for (int i = x + 1; i <= x + vel; ++i) {
				if ((i == (x + 1)) && isIgnoreCommander && field[i][y] == rc) {
					++steps;
					continue;
				}

				if (i >= field.size())
					return steps;
				if (field[i][y] != ai_seen)
					return steps;
				++steps;
			}
		}
		else if (dist == left) {
			for (int i = x - 1; i >= x - vel; --i) {
				if ((i == (x - 1)) && isIgnoreCommander && field[i][y] == rc) {
					++steps;
					continue;
				}

				if (i < 0)
					return steps;

				if (field[i][y] != ai_seen) {
					return steps;
				}
				++steps;
			}
		}
		else if (dist == down) {
			for (int i = y + 1; i <= y + vel; ++i) {
				if ((i == (y + 1)) && isIgnoreCommander && field[x][i] == rc) {
					++steps;
					continue;
				}
				if (i >= field[0].size())
					return steps;
				if (field[x][i] != ai_seen)
					return steps;
				++steps;
			}
		}
		else if (dist == up) {
			for (int i = y - 1; i >= y - vel; --i) {
				if ((i == (y - 1)) && isIgnoreCommander && field[x][i] == rc) {
					++steps;
					continue;
				}
				if (i < 0)
					return steps;
				if (field[x][i] != ai_seen)
					return steps;
				++steps;
			}
		}
		return steps;
	}

	void showfield(Field_size field_Size, vector<vector<unsigned>>& field, vector<Point>& pointsOfInterest = no_vector) {
		if (field_Size.m == 0 || field_Size.n == 0)
			throw std::exception(">>> Environment is empty");
		for (int j = 0; j < field_Size.m; ++j) {
			for (int i = 0; i < field_Size.n; ++i) {
				switch (field[i][j]) {
				case none_cell:
					std::cout << "x";
					break;
				case border:
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

	void show(ED_N::environmentDescriptor* environment, vector<vector<unsigned>>& field) {
		if (IS_ACTIVE) {
			if (system("CLS"))
				system("clear");
			showfield(environment->getSize(), field);
			//std::cout << "\n\n Step: "<<num<<"\n\n";
			std::cout << "\n Real Map:\n\n";
			environment->drawJustMap();
		}
	}

	vector<std::stack<Point>> way_back;


	void AI::bfs(int c_num, Point v, Point e, vector<vector<unsigned>>& field) {
		queue<Point> q;
		Point s = v;
		vector<Point> p;
		vector<vector<int>> mark;
		std::map<Point, vector<Vertex>> _n_graph;

		int i = s.x;
		int j = s.y;




		for (int i = 0; i < field.size(); ++i) {
			mark.push_back(vector<int>());
			for (int j = 0; j < field[i].size(); ++j) {
				mark[i].push_back(0);
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
					if ((j < field[0].size() - 1 && (field[i][j + 1] == ai_seen)) || (potential_end == e))
						nVertex.push_back({ i, j + 1 });

					potential_end = { i + 1, j };
					if ((i < field.size() - 1 && (field[i + 1][j] == ai_seen)) || (potential_end == e))
						nVertex.push_back({ i + 1, j });

					_n_graph.insert(pair<Point, vector<Vertex>>({ i, j }, nVertex));
				}
			}
		}

		for (size_t j = 0; j < field.size() * field[0].size(); j++)
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
				if (mark[(*iter).x][(*iter).y]) {
					continue;
				}
				q.push({ (*iter).x,(*iter).y });
				mark[(*iter).x][(*iter).y] = true;
				int num = (*iter).y * field.size() + (*iter).x;
				p[num] = v;
			}
		}

		if (!mark[e.x][e.y]) {
			return;
		}
		else {
			int num = e.y * field.size() + e.x;
			int end = s.y * field.size() + s.x;
			while (num != end) {
				way_back[c_num].push(p[num]);
				num = p[num].y * field.size() + p[num].x;
			}
			way_back[c_num].pop();
		};
		return;
	}

	vector<vector<stack<Point>>> _stacks;
	void AI::dfs(ED_N::environmentDescriptor* environment, vector<vector<unsigned>>& field, vector<Components_N::managementComponent*> components) {

		const int FPS = 60;
		const int frameDelay = 1000 / FPS;
		Uint32 frameStart;
		int frameTime;

		render* _render = new render("NameOfGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 946, false);
		_render->play();
		vector<Component*> dead_robots;
		vector<stack<Point>> _dead_stacks;
		vector<Point> active_points;
		vector<Component*> UselessObserveCenters;

		vector<char const*> _budget;



		char* _string12 = new char[12];
		strcpy_s(_string12, 12, "Start");
		_budget.push_back(_string12);

		std::string s = std::to_string(budget);
		char const* _string = s.c_str();
		_budget.push_back(_string);

		vector <stack<unsigned>> _states;

		bool scanning;

		for (size_t i = 0; i < components.size(); i++)
		{
			way_back.push_back(std::stack<Point>());
		}

		bool _ai_ = true;

		vector<Components_N::managementComponent*>::iterator component;
		while (_ai_) {
			_ai_ = false;
			int c_n = 0;


			++num;

			component = components.begin();

			for (size_t i = 0; i < components.size(); i++)
			{
				if (components[i]->getActive() == true)
					_ai_ = true;
			}

			if (TryClear) {
				//vector<vector<unsigned>> _field = field;
				bool stop = true;
				for (size_t i = 0; i < field.size(); i++)
				{
					for (size_t j = 0; j < field[0].size(); j++)
					{
						if (field[i][j] == ai_seen) {
							//check left
							if ((i != 0) && (i > 0 && (field[i - 1][j] == notexist)))
								stop = false;
							//check up
							if ((j != 0) && (j > 0 && (field[i][j - 1] == notexist)))
								stop = false;
							//check right
							if ((i != field.size() - 1) && (i < field.size() - 1 && (field[i + 1][j] == notexist)))
								stop = false;
							//check down
							if ((j != field[0].size() - 1) & (j < field[0].size() - 1 && (field[i][j + 1] == notexist)))
								stop = false;
						}
					}
				}
				if (stop) {
					_ai_ = false;
					continue;

				}
			}

			map<Point, Point> prev;

			frameStart = SDL_GetTicks();
			while (component != components.end())
			{


				//show(environment, field);
				//	showfield(environment->getSize(), visited_vertex);

				bool comander = false;
				if ((*component)->iAm() == robot_commander)
					comander = true;

				if ((*component)->getActive() == false) {

					int x2 = (*component)->getCoord().x;
					int y2 = (*component)->getCoord().y;

					int radius;
					vector<Modules_N::Module*>::iterator it = (*component)->getModules()->begin();
					while (it != (*component)->getModules()->end())
					{
						if ((*it)->iAm() == management_Module) {
							radius = dynamic_cast<managementModule*>(*it)->getR();
							break;
						}
						it++;
					}
					//SCANNING DEAD
					for (size_t i = 0; i < dead_robots.size(); i++)
					{
						if ((*component)->getNComp()->size() == (*component)->getNum())
							break;
						int x = dead_robots[i]->getCoord().x;
						int y = dead_robots[i]->getCoord().y;


						if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius) {
							(*component)->setActive();
							_states[c_n].push(waiting_mode);
							while (it != (*component)->getModules()->end())
							{
								if ((*it)->iAm() == management_Module) {
									dynamic_cast<managementModule*>(*it)->sendResourse((*component), dead_robots[i]);
									_stacks[c_n].push_back(_dead_stacks[i]);
									field[dead_robots[i]->getCoord().x][dead_robots[i]->getCoord().y] = rs;

									vector<Component*>::iterator temp = dead_robots.begin();
									vector<stack<Point>>::iterator temp2 = _dead_stacks.begin();

									_dead_stacks.erase(temp2 + i);
									dead_robots.erase(temp + i);
									--i;
									break;
								}
								it++;
							}
							break;
						}

						if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius) {
							this->bfs(c_n, { x2, y2 }, { x,y }, field);
							_states[c_n].push(following_mode);

						}
					}

					++component;
					++c_n;
					continue;
				}

				if (_states.size() == c_n) {
					_states.push_back(stack<unsigned>());
					_states[c_n].push(waiting_mode);
				}

				scanning = false;

				int x2 = (*component)->getCoord().x;
				int y2 = (*component)->getCoord().y;

				//WAITING_MODE
				if (_states[c_n].top() == waiting_mode) {
					int radius = 0;
					vector<Modules_N::Module*>::iterator it = (*component)->getModules()->begin();
					while (it != (*component)->getModules()->end())
					{
						if ((*it)->iAm() == management_Module) {
							radius = dynamic_cast<managementModule*>(*it)->getR();
							break;
						}
						it++;
					}

					int save;
					if (comander)
						save = -1;
					else
						save = -2;

					//SCANNING DEAD
					for (size_t i = 0; i < dead_robots.size(); i++)
					{
						if ((*component)->getNumD() <= (*component)->getNum())
							break;
						int x = dead_robots[i]->getCoord().x;
						int y = dead_robots[i]->getCoord().y;
						if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius) {
							while (it != (*component)->getModules()->end())
							{
								if ((*it)->iAm() == management_Module) {
									dynamic_cast<managementModule*>(*it)->sendResourse((*component), dead_robots[i]);
									_stacks[c_n].push_back(_dead_stacks[i]);
									field[dead_robots[i]->getCoord().x][dead_robots[i]->getCoord().y] = rs;

									vector<Component*>::iterator temp = dead_robots.begin();
									vector<stack<Point>>::iterator temp2 = _dead_stacks.begin();

									_dead_stacks.erase(temp2 + i);
									dead_robots.erase(temp + i);
									--i;
									break;
								}
								it++;
							}
							break;
						}

						if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius) {
							this->bfs(c_n, { x2, y2 }, { x,y }, field);
							_states[c_n].push(following_mode);

						}
					}

					//SCANNING OC
					for (size_t i = 0; i < UselessObserveCenters.size(); i++)
					{
						if ((*component)->getNumD() <= (*component)->getNComp()->size())
							break;

						int x = UselessObserveCenters[i]->getCoord().x;
						int y = UselessObserveCenters[i]->getCoord().y;
						if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius)
							break;
						if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius) {
							while (it != (*component)->getModules()->end())
							{
								if ((*it)->iAm() == management_Module) {
									dynamic_cast<managementModule*>(*it)->sendResourse((*component), UselessObserveCenters[i]);

									EnvironmentInfo env = (*component)->getInfo((*component)->getNComp()->size() - 1, environment, field);
									vector<Component*> diffComponents = env.components;

									//ADDITION TO SCAN 
									vector<Point>::iterator allpoints = env.allpoints.begin();
									for (allpoints; allpoints != env.allpoints.end(); allpoints++)
									{
										//check left
										if (((*allpoints).x == 0) || ((*allpoints).x > 0 && (field[(*allpoints).x - 1][(*allpoints).y] != notexist)))
											//check up
											if (((*allpoints).y == 0) || ((*allpoints).y > 0 && (field[(*allpoints).x][(*allpoints).y - 1] != notexist)))
												//check right
												if (((*allpoints).x == field.size() - 1) || ((*allpoints).x < field.size() - 1 && (field[(*allpoints).x + 1][(*allpoints).y] != notexist)))
													//check down
													if (((*allpoints).y == field[0].size() - 1) || ((*allpoints).y < field[0].size() - 1 && (field[(*allpoints).x][(*allpoints).y + 1] != notexist))) {
														//CHECK CELLS AROUND THIS POINT

														int x = (*allpoints).x;
														int y = (*allpoints).y;

														Point vertex = { x, y };

														vector<Vertex> newVertex;
														Point test_point;
														test_point = { x - 1,y };
														if ((x > 0) && ((field[x - 1][y] == ai_seen) || (field[x - 1][y] == rc)) && visited_vertex[x - 1][y] == V_not)
															newVertex.push_back({ x - 1, y });
														test_point = { x,y - 1 };
														if ((y > 0) && ((field[x][y - 1] == ai_seen) || (field[x][y - 1] == rc)) && visited_vertex[x][y - 1] == V_not)
															newVertex.push_back({ x, y - 1 });
														test_point = { x,y + 1 };
														if ((y < environment->getSize().m - 1) && ((field[x][y + 1] == ai_seen) || (field[x][y + 1] == rc)) && visited_vertex[x][y + 1] == V_not)
															newVertex.push_back({ x, y + 1 });
														test_point = { x + 1,y };
														if ((x < environment->getSize().n - 1) && ((field[x + 1][y] == ai_seen) || (field[x + 1][y] == rc)) && visited_vertex[x + 1][y] == V_not)
															newVertex.push_back({ x + 1, y });

														_graph.insert(pair<Point, vector<Vertex>>(vertex, newVertex));
													}
									}

									vector<Component*>::iterator it0 = diffComponents.begin();
									while (it0 != diffComponents.end())
									{
										if (((*it0)->iAm() == observe_center) && (visited_vertex[(*it0)->getCoord().x][(*it0)->getCoord().y] != V_visited)) {
											UselessObserveCenters.push_back(*it0);
											visited_vertex[(*it0)->getCoord().x][(*it0)->getCoord().y] = V_visited;
											field[(*it0)->getCoord().x][(*it0)->getCoord().y] = oc;
										}
										else if (((*it0)->iAm() == robot_scout) && (*it0)->getActive() == false) {
											dead_robots.push_back((*it0));
											(*it0)->setActive();
											_dead_stacks.push_back(std::stack<Point>());
											_dead_stacks[_dead_stacks.size() - 1].push((*it0)->getCoord());
											field[(*it0)->getCoord().x][(*it0)->getCoord().y] = rsd;
										}
										++it0;
									}
									dynamic_cast<managementModule*>(*it)->freeResourse((*component), (*component)->getNComp()->size() - 1);
									vector<Component*>::iterator temp = UselessObserveCenters.begin();
									if (UselessObserveCenters.size() > 1)
										UselessObserveCenters.erase(temp + i);
									else
										UselessObserveCenters.clear();
								}
								++it;
							}
						}
					}

					//SCANING COMPONENTS
					for (size_t j = 0; j < (*component)->getNComp()->size(); j++)
					{
						int x = (*component)->getComp(j)->getCoord().x;
						int y = (*component)->getComp(j)->getCoord().y;
						Point nopoint = { -1, -1 };
						if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius) && (save == -1)) {
							save = j;
						}
						else if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > radius) && (save != -1)) {
							vector<Modules_N::Module*>::iterator it = (*component)->getModules()->begin();
							while (it != (*component)->getModules()->end())
							{
								if ((*it)->iAm() == management_Module) {
									dead_robots.push_back((*component)->getComp(j));
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rsd;
									dynamic_cast<managementModule*>(*it)->freeResourse((*component), j);
									_dead_stacks.push_back(_stacks[c_n][j]);
									_dead_stacks[0].push({ x,y });
									vector<stack<Point>>::iterator temp2 = _stacks[c_n].begin();
									_stacks[c_n].erase(temp2 + j);
									--j;
									break;
								}
								it++;
							}
						}

						else if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius) && (save == -1))
							save = -2;

						//GIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIVE A CHANCE
						else if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius) && (save >= 0)) {
							if ((!_stacks[c_n][j].empty()) && (_stacks[c_n][j].top() != nopoint)) {
								vector<Modules_N::Module*>::iterator it = (*component)->getModules()->begin();
								while (it != (*component)->getModules()->end()) {
									if ((*it)->iAm() == management_Module) {
										dead_robots.push_back((*component)->getComp(save));
										field[(*component)->getComp(save)->getCoord().x][(*component)->getComp(save)->getCoord().y] = rsd;
										dynamic_cast<managementModule*>(*it)->freeResourse((*component), save);
										_dead_stacks.push_back(_stacks[c_n][save]);

										_dead_stacks[0].push({ x,y });
										vector<stack<Point>>::iterator temp2 = _stacks[c_n].begin();
										_stacks[c_n].erase(temp2 + save);
										save = -2;
										--j;
										break;
									}
									++it;
								}
							}
							else {
								vector<Modules_N::Module*>::iterator it = (*component)->getModules()->begin();
								while (it != (*component)->getModules()->end()) {
									if ((*it)->iAm() == management_Module) {
										dead_robots.push_back((*component)->getComp(save));
										field[(*component)->getComp(save)->getCoord().x][(*component)->getComp(save)->getCoord().y] = rsd;
										dynamic_cast<managementModule*>(*it)->freeResourse((*component), save);
										_dead_stacks.push_back(_stacks[c_n][save]);

										_dead_stacks[0].push({ x,y });
										vector<stack<Point>>::iterator temp2 = _stacks[c_n].begin();
										_stacks[c_n].erase(temp2 + save);
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
						int x = (*component)->getComp(save)->getCoord().x;
						int y = (*component)->getComp(save)->getCoord().y;

						this->bfs(c_n, { x2, y2 }, { x,y }, field);
						_states[c_n].push(following_mode);
						scanning = true;
						++component;
						++c_n;
						continue;
					}
				}

				//FOLLOW
				else if (_states[c_n].top() == following_mode) {
					int x = (*component)->getComp(0)->getCoord().x;
					int y = (*component)->getComp(0)->getCoord().y;
					int x2 = (*component)->getCoord().x;
					int y2 = (*component)->getCoord().y;

					if (way_back[c_n].empty()) {
						_states[c_n].pop();
						scanning = true;
						++component;
						++c_n;
						continue;
					}

					if ((sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)) > 1)) {
						if ((way_back[c_n].top().x > x2) && checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel())) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(right, checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel()));
							field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
						}
						else if ((way_back[c_n].top().x < x2) && checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel())) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(left, checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel()));
							field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
						}
						else if ((way_back[c_n].top().y > y2) && checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel())) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(down, checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel()));
							field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
						}
						else if ((way_back[c_n].top().y < y2) && checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel())) {
							field[x2][y2] = ai_seen;
							dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(up, checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel()));
							field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
						}

						prev.insert(pair<Point, Point>({ x2,y2 }, (*component)->getCoord()));

						way_back[c_n].pop();

						scanning = true;
						++component;
						++c_n;
						continue;
					}
					else {
						while (!way_back[c_n].empty())
							way_back[c_n].pop();

						_states[c_n].pop();

						scanning = true;
						++component;
						++c_n;
						continue;
					}
				}

				//STATE
				else if (_states[c_n].top() == state_mode)
					if (dead_robots.size() != 0) {

						size_t i = 0;
						for (; i < dead_robots.size(); i++)
						{

							int x = dead_robots[i]->getCoord().x;
							int y = dead_robots[i]->getCoord().y;

							this->bfs(c_n, { x2, y2 }, { x,y }, field);
							if (!way_back[c_n].empty())
								break;
						}

						if (!way_back[c_n].empty()) {
							vector<Modules_N::Module*>::iterator it = (*component)->getModules()->begin();
							it = (*component)->getModules()->begin();
							while (it != (*component)->getModules()->end())
							{
								if ((*it)->iAm() == management_Module) {
									dynamic_cast<managementModule*>(*it)->sendResourse((*component), dead_robots[i]);
									_stacks[c_n].push_back(_dead_stacks[i]);
									field[dead_robots[i]->getCoord().x][dead_robots[i]->getCoord().y] = rs;
									vector<Component*>::iterator temp = dead_robots.begin();
									vector<stack<Point>>::iterator temp2 = _dead_stacks.begin();

									_dead_stacks.erase(temp2 + i);
									dead_robots.erase(temp + i);
									break;
								}
								it++;
							}


							scanning = true;
							_states[c_n].pop();
							_states[c_n].push(following_mode);
							++component;
							++c_n;
							continue;
						}
					}

				//COMPONENTS
				for (size_t j = 0; j < (*component)->getNComp()->size(); j++)
				{


					Point nopoint = { -1,-1 };
					if ((_stacks[c_n][j].size()) && (_stacks[c_n][j].top() == nopoint)) {
						field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rsd;


						for (int i = (*component)->getComp(j)->getModules()->size(); i > 0; i--)
						{

							budget += (*component)->getComp(j)->getModule(0)->getCost();
							shop.push_back((*component)->getComp(j)->getModule(0));

							_budget.push_back("");

							char* _string1 = new char[12];
							strcpy_s(_string1, 12, "+");
							char* _string2 = new char[10];
							sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(0)->getCost());
							strcat_s(_string1, 12, _string2);
							_budget.push_back(_string1);

							char* _string12 = new char[12];
							strcpy_s(_string12, 12, "Sensor sold");
							_budget.push_back(_string12);

							(*component)->getComp(j)->deleteModule(0);


						}
						continue;
					}

					//CHECK SHOP
					if (shop.size() != 0) {
						//
						//	CHECKING GENERATOR
						//

						int check_man = -1;
						int check_generator = -1;
						int check_sensor = -1;
						int shop_generator = 0;
						int shop_man = -1;

						int num = 0;

						vector<Module*>::iterator m_iter = (*component)->getComp(j)->getModules()->begin();
						for (m_iter; m_iter != (*component)->getComp(j)->getModules()->end(); ++m_iter, ++num)
						{
							if ((*m_iter)->iAm() == generator_Module) {
								if (check_generator < 0) {
									check_generator = num;
								}
								else {

									int s = dynamic_cast<generatorModule*>((*component)->getComp(j)->getModule(check_generator))->getEnergyProvision();
									int new_s = dynamic_cast<generatorModule*>((*component)->getComp(j)->getModule(num))->getEnergyProvision();

									// IF NEW SENSOR CAS SEE FAR
									if (new_s > s) {

										//SELL S2
										budget += (*component)->getComp(j)->getModule(check_generator)->getCost();
										shop.push_back((*component)->getComp(j)->getModule(check_generator));

										_budget.push_back("");

										char* _string1 = new char[12];
										strcpy_s(_string1, 12, "+");
										char* _string2 = new char[10];
										sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(check_generator)->getCost());
										strcat_s(_string1, 12, _string2);
										_budget.push_back(_string1);

										char* _string12 = new char[12];
										strcpy_s(_string12, 12, "Sensor sold");
										_budget.push_back(_string12);

										(*component)->getComp(j)->deleteModule(check_generator);

										if (check_man > check_generator)
											--check_man;
										if (check_sensor > check_generator)
											--check_sensor;

										check_generator = num - 1;
										--num;
										--m_iter;
										continue;

									}
									else {

										//SELL S2
										budget += (*component)->getComp(j)->getModule(num)->getCost();
										shop.push_back((*component)->getComp(j)->getModule(num));
										_budget.push_back("");

										char* _string1 = new char[12];
										strcpy_s(_string1, 12, "+");
										char* _string2 = new char[10];
										sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(num)->getCost());
										strcat_s(_string1, 12, _string2);
										_budget.push_back(_string1);

										char* _string12 = new char[12];
										strcpy_s(_string12, 12, "Sensor sold");
										_budget.push_back(_string12);

										(*component)->getComp(j)->deleteModule(num);
										--num;
										--m_iter;
										continue;
									}
								}
							}

							if ((*m_iter)->iAm() == sensor_Module) {

								if (check_sensor < 0) {
									check_sensor = num;
								}
								else {

									int s = (dynamic_cast<sensorModule*>((*component)->getComp(j)->getModule(check_sensor))->getAng())* (dynamic_cast<sensorModule*>((*component)->getComp(j)->getModule(check_sensor))->getR());
									int new_s = (dynamic_cast<sensorModule*>((*component)->getComp(j)->getModule(num))->getAng())* (dynamic_cast<sensorModule*>((*component)->getComp(j)->getModule(num))->getR());

									// IF NEW SENSOR CAS SEE FAR
									if (new_s > s) {

										//SELL S2
										budget += (*component)->getComp(j)->getModule(check_sensor)->getCost();
										shop.push_back((*component)->getComp(j)->getModule(check_sensor));

										_budget.push_back("");

										char* _string1 = new char[12];
										strcpy_s(_string1, 12, "+");
										char* _string2 = new char[10];
										sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(check_sensor)->getCost());
										strcat_s(_string1, 12, _string2);
										_budget.push_back(_string1);

										char* _string12 = new char[12];
										strcpy_s(_string12, 12, "Sens sold");
										_budget.push_back(_string12);

										(*component)->getComp(j)->deleteModule(check_sensor);

										if (check_generator > check_sensor)
											--check_generator;
										if (check_man > check_sensor)
											--check_man;

										check_sensor = num - 1;
										--num;
										--m_iter;
										continue;

									}
									else {

										//SELL S2
										budget += (*component)->getComp(j)->getModule(num)->getCost();
										shop.push_back((*component)->getComp(j)->getModule(num));
										_budget.push_back("");

										char* _string1 = new char[12];
										strcpy_s(_string1, 12, "+");
										char* _string2 = new char[10];
										sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(num)->getCost());
										strcat_s(_string1, 12, _string2);
										_budget.push_back(_string1);

										char* _string12 = new char[12];
										strcpy_s(_string12, 12, "Sens sold");
										_budget.push_back(_string12);

										(*component)->getComp(j)->deleteModule(num);
										--num;
										--m_iter;
										continue;
									}
								}
							}

							/*if ((*m_iter)->iAm() == management_Module) {

								if (check_man < 0) {
									check_man = num;
								}
								else {

									int s = (dynamic_cast<managementModule*>((*component)->getComp(j)->getModule(check_man))->getN())* (dynamic_cast<managementModule*>((*component)->getComp(j)->getModule(check_man))->getR());
									int new_s = (dynamic_cast<managementModule*>((*component)->getComp(j)->getModule(num))->getN())* (dynamic_cast<managementModule*>((*component)->getComp(j)->getModule(num))->getR());

									// IF NEW SENSOR CAS SEE FAR
									if (new_s > s) {

										//SELL S2
										budget += (*component)->getComp(j)->getModule(check_man)->getCost();
										shop.push_back((*component)->getComp(j)->getModule(check_man));

										_budget.push_back("");

										char* _string1 = new char[12];
										strcpy_s(_string1, 12, "+");
										char* _string2 = new char[10];
										sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(check_man)->getCost());
										strcat_s(_string1, 12, _string2);
										_budget.push_back(_string1);

										char* _string12 = new char[12];
										strcpy_s(_string12, 12, "Sensor sold");
										_budget.push_back(_string12);

										(*component)->getComp(j)->deleteModule(check_man);

										if (check_generator > check_man)
											--check_generator;
										if (check_sensor > check_man)
											--check_sensor;

										check_man = num - 1;
										--num;
										--m_iter;
										continue;

									}
									else {

										//SELL S2
										budget += (*component)->getComp(j)->getModule(num)->getCost();
										shop.push_back((*component)->getComp(j)->getModule(num));
										_budget.push_back("");

										char* _string1 = new char[12];
										strcpy_s(_string1, 12, "+");
										char* _string2 = new char[10];
										sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(num)->getCost());
										strcat_s(_string1, 12, _string2);
										_budget.push_back(_string1);

										char* _string12 = new char[12]; 
										strcpy_s(_string12, 12, "Sensor sold");
										_budget.push_back(_string12);

										(*component)->getComp(j)->deleteModule(num);
										--num;
										--m_iter;
										continue;
									}
								}
							}*/

						}

						if (check_generator == -1) {

							vector<Module*>::iterator m_shop_iter = shop.begin();
							for (m_shop_iter; m_shop_iter != shop.end(); ++m_shop_iter)
							{
								if ((*m_shop_iter)->iAm() == generator_Module)
									break;
								++shop_generator;
							}

							if ((shop_generator != -1) && budget > shop[shop_generator]->getCost()) {
								if ((*component)->getComp(j)->getModules()->size() < (*component)->getComp(j)->getSlotsNum()) {

									(*component)->getComp(j)->setModule_g(dynamic_cast<generatorModule*>(shop[shop_generator]));

									//Budget setting
									budget -= shop[shop_generator]->getCost();

									_budget.push_back("");

									char* _string1 = new char[12];
									strcpy_s(_string1, 12, "-");
									char* _string2 = new char[10];
									sprintf_s(_string2, 10, "%d", shop[shop_generator]->getCost());
									strcat_s(_string1, 12, _string2);
									_budget.push_back(_string1);

									char* _string12 = new char[12];
									strcpy_s(_string12, 12, "Gen bought");
									_budget.push_back(_string12);

									shop.erase(shop.begin() + shop_generator);
									check_generator = (*component)->getComp(j)->getModules()->size() - 1;
								}
							}
						}

						if (check_sensor == -1) {

							int i = 0;

							vector<Module*>::iterator m_shop_iter = shop.begin();
							for (m_shop_iter; m_shop_iter != shop.end(); ++m_shop_iter)
							{
								if ((*m_shop_iter)->iAm() == robot_scout)
									break;
								++i;
							}

							(*component)->getComp(j)->setModule_s(dynamic_cast<sensorModule*>(shop[i]));

							(*component)->getComp(j)->moduleOn((*component)->getComp(j)->getModules()->size() - 1);

							//
							// BUY SENSOR
							//

							vector<Module*>::iterator iter = shop.begin();

							_budget.push_back("");

							char* e_string1 = new char[12];
							strcpy_s(e_string1, 12, "-");
							char* e_string2 = new char[10];
							sprintf_s(e_string2, 10, "%d", shop[i]->getCost());
							strcat_s(e_string1, 12, e_string2);
							_budget.push_back(e_string1);

							char* e_string12 = new char[12];
							strcpy_s(e_string12, 12, "for sens");
							_budget.push_back(e_string12);

							budget -= shop[i]->getCost();

							char* e_string21 = new char[12];
							strcpy_s(e_string21, 12, "=");
							char* e_string22 = new char[10];
							sprintf_s(e_string22, 10, "%d", budget);
							strcat_s(e_string21, 12, e_string22);
							_budget.push_back(e_string21);

							shop.erase(iter + i);

							(*component)->getComp(j)->moduleOn((*component)->getComp(j)->getModules()->size() - 1);
							check_sensor = (*component)->getComp(j)->getModules()->size() - 1;
						}

						/*if (check_man == -1) {

							vector<Module*>::iterator m_shop_iter = shop.begin();
							for (m_shop_iter; m_shop_iter != shop.end(); ++m_shop_iter)
							{
								if ((*m_shop_iter)->iAm() == management_Module)
									break;
								++shop_man;
							}

							if ((check_man != -1) && budget > shop[shop_man]->getCost()) {
								if ((*component)->getComp(j)->getModules()->size() < (*component)->getComp(j)->getSlotsNum()) {
									(*component)->getComp(j)->setModule_s(dynamic_cast<sensorModule*>(shop[shop_man]));
									(*component)->getComp(j)->moduleOn((*component)->getComp(j)->getModules()->size() - 1);

									//Budget setting
									budget -= shop[shop_man]->getCost();

									_budget.push_back("");

									char* _string1 = new char[12];
									strcpy_s(_string1, 12, "-");
									char* _string2 = new char[10];
									sprintf_s(_string2, 10, "%d", shop[shop_man]->getCost());
									strcat_s(_string1, 12, _string2);
									_budget.push_back(_string1);

									char* _string12 = new char[12];
									strcpy_s(_string12, 12, "Man bought");
									_budget.push_back(_string12);

									shop.erase(shop.begin() + shop_man);
									check_man = (*component)->getComp(j)->getModules()->size() - 1;
								}
							}
						}*/


						for (size_t i = 0; i < shop.size(); i++)
						{

							if ((shop[i]->getCost() < budget) && shop[i]->iAm() == sensor_Module) {


								int s = (dynamic_cast<sensorModule*>((*component)->getComp(j)->getModule(check_sensor))->getAng())* (dynamic_cast<sensorModule*>((*component)->getComp(j)->getModule(check_sensor))->getR());
								int new_s = (dynamic_cast<sensorModule*>(shop[i])->getAng()) * (dynamic_cast<sensorModule*>(shop[i])->getR());

								if (new_s > s) {

									//
									// SELL SENSOR
									//

									budget += (*component)->getComp(j)->getModule(check_sensor)->getCost();
									shop.push_back((*component)->getComp(j)->getModule(check_sensor));


									_budget.push_back("");

									char* _string1 = new char[12];
									strcpy_s(_string1, 12, "+");
									char* _string2 = new char[10];
									sprintf_s(_string2, 10, "%d", (*component)->getComp(j)->getModule(check_sensor)->getCost());
									strcat_s(_string1, 12, _string2);
									_budget.push_back(_string1);

									char* _string12 = new char[12];
									strcpy_s(_string12, 12, "SM sold");
									_budget.push_back(_string12);

									(*component)->getComp(j)->deleteModule(check_sensor);

									//
									// SET NEW
									//


									(*component)->getComp(j)->setModule_s(dynamic_cast<sensorModule*>(shop[i]));

									(*component)->getComp(j)->moduleOn((*component)->getComp(j)->getModules()->size() - 1);


									//
									// BUY SENSOR
									//

									vector<Module*>::iterator iter = shop.begin();

									_budget.push_back("");

									char* e_string1 = new char[12];
									strcpy_s(e_string1, 12, "-");
									char* e_string2 = new char[10];
									sprintf_s(e_string2, 10, "%d", shop[i]->getCost());
									strcat_s(e_string1, 12, e_string2);
									_budget.push_back(e_string1);

									char* e_string12 = new char[12];
									strcpy_s(e_string12, 12, "for sens");
									_budget.push_back(e_string12);

									budget -= shop[i]->getCost();

									char* e_string21 = new char[12];
									strcpy_s(e_string21, 12, "=");
									char* e_string22 = new char[10];
									sprintf_s(e_string22, 10, "%d", budget);
									strcat_s(e_string21, 12, e_string22);
									_budget.push_back(e_string21);

									shop.erase(iter + i);

									(*component)->getComp(j)->moduleOn((*component)->getComp(j)->getModules()->size() - 1);
									check_sensor = (*component)->getComp(j)->getModules()->size() - 1;
									i--;
								}
							}
						}
					}

					Point vertex = { (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y };

					int x = (*component)->getComp(j)->getCoord().x;
					int y = (*component)->getComp(j)->getCoord().y;

					//Coord_Set
					if ((*component)->iAm() == robot_commander)
						field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
					else if ((*component)->iAm() == command_center)
						field[(*component)->getCoord().x][(*component)->getCoord().y] = cc;

					field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;

					//SCAN VERTEX
					if (visited_vertex[vertex.x][vertex.y] == V_not) {
						visited_vertex[vertex.x][vertex.y] = V_partly;
						EnvironmentInfo env;
						vector<Component*> diffComponents;
						vector<Point> allpoints_g;
						//GET ALL INFO FROM ROBOT
						for (size_t i = 0; i < 4; i++)
						{
							env = (*component)->getInfo(j, environment, field);
							diffComponents = env.components;


							vector<Point>::iterator iter;
							iter = env.allpoints.begin();
							while (iter != env.allpoints.end())
							{
								if (std::find(allpoints_g.begin(), allpoints_g.end(), *iter) == allpoints_g.end()) {
									allpoints_g.push_back(*iter);
								}
								++iter;
							}

							vector<Component*>::iterator it0 = diffComponents.begin();
							while (it0 != diffComponents.end())
							{
								if (((*it0)->iAm() == observe_center) && (visited_vertex[(*it0)->getCoord().x][(*it0)->getCoord().y] != V_visited)) {
									UselessObserveCenters.push_back(*it0);
									visited_vertex[(*it0)->getCoord().x][(*it0)->getCoord().y] = V_visited;
									field[(*it0)->getCoord().x][(*it0)->getCoord().y] = oc;
								}
								else if (((*it0)->iAm() == robot_scout) && (*it0)->getActive() == false) {
									dead_robots.push_back((*it0));
									(*it0)->setActive();
									_dead_stacks.push_back(std::stack<Point>());
									_dead_stacks[_dead_stacks.size() - 1].push((*it0)->getCoord());
									field[(*it0)->getCoord().x][(*it0)->getCoord().y] = rsd;
								}
								++it0;
							}

							vector<Modules_N::Module*>::iterator it = (*component)->getComp(j)->getModules()->begin();
							while (it != (*component)->getComp(j)->getModules()->end())
							{
								if ((*it)->iAm() == sensor_Module)
									dynamic_cast<sensorModule*>(*it)->turn();
								++it;
							}
						}


						//ADDITION TO SCAN 
						vector<Point>::iterator allpoints = allpoints_g.begin();
						for (allpoints; allpoints != allpoints_g.end(); allpoints++)
						{
							//check left
							if (((*allpoints).x == 0) || ((*allpoints).x > 0 && (field[(*allpoints).x - 1][(*allpoints).y] != notexist)))
								//check up
								if (((*allpoints).y == 0) || ((*allpoints).y > 0 && (field[(*allpoints).x][(*allpoints).y - 1] != notexist)))
									//check right
									if (((*allpoints).x == field.size() - 1) || ((*allpoints).x < field.size() - 1 && (field[(*allpoints).x + 1][(*allpoints).y] != notexist)))
										//check down
										if (((*allpoints).y == field[0].size() - 1) || ((*allpoints).y < field[0].size() - 1 && (field[(*allpoints).x][(*allpoints).y + 1] != notexist))) {

											//CHECK CELLS AROUND THIS POINT

											int x = (*allpoints).x;
											int y = (*allpoints).y;

											int n = 0;

											if ((x == 0) || ((x > 0) && ((field[x - 1][y] == interest_point) || (field[x - 1][y] == border) || (field[x - 1][y] == barrier)))) {
												++n;
											}

											if ((y == 0) || ((y > 0) && ((field[x][y - 1] == interest_point) || (field[x][y - 1] == border) || (field[x][y - 1] == barrier)))) {
												++n;
											}

											if ((y == environment->getSize().m - 1) || ((y < environment->getSize().m - 1) && (field[x][y + 1] == interest_point || field[x][y + 1] == border || (field[x][y + 1] == barrier)))) {
												++n;
											}

											if ((x == environment->getSize().n - 1) || ((x < environment->getSize().n - 1) && (field[x + 1][y] == interest_point || field[x + 1][y] == border || (field[x + 1][y] == barrier)))) {
												++n;
											}

											if (n > 2)
												visited_vertex[x][y] = V_visited;
										}
						}


						//CHECK CELLS AROUND THE ROBOT
						vector<Vertex> newVertex;
						Point test_point;
						test_point = { x - 1,y };
						if ((x > 0) && ((field[x - 1][y] == ai_seen) || (field[x - 1][y] == rc)) && visited_vertex[x - 1][y] == V_not)
							newVertex.push_back({ x - 1, y });
						test_point = { x,y - 1 };
						if ((y > 0) && ((field[x][y - 1] == ai_seen) || (field[x][y - 1] == rc)) && visited_vertex[x][y - 1] == V_not)
							newVertex.push_back({ x, y - 1 });
						test_point = { x,y + 1 };
						if ((y < environment->getSize().m - 1) && ((field[x][y + 1] == ai_seen) || (field[x][y + 1] == rc)) && visited_vertex[x][y + 1] == V_not)
							newVertex.push_back({ x, y + 1 });
						test_point = { x + 1,y };
						if ((x < environment->getSize().n - 1) && ((field[x + 1][y] == ai_seen) || (field[x + 1][y] == rc)) && visited_vertex[x + 1][y] == V_not)
							newVertex.push_back({ x + 1, y });

						//IF NO NEW WAYS -> V == visited
						if (newVertex.size() == 0) {
							visited_vertex[vertex.x][vertex.y] = V_visited;
						}

						_graph.insert(pair<Point, vector<Vertex>>(vertex, newVertex));
						_stacks[c_n][j].push(vertex);
					}

					//SCAN WAYS
					if (visited_vertex[vertex.x][vertex.y] == V_partly) {
						if (_stacks[c_n][j].top() != vertex)
							_stacks[c_n][j].push(vertex);
						vector<Vertex>::iterator iter;
						iter = _graph[vertex].begin();
						int ways_left = false;
						for (iter; iter != _graph[vertex].end(); iter++)
						{
							if ((*iter).visited == V_not) {
								ways_left = true;
								(*iter).visited = V_visited;

								if (((*iter).x < x) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, left, (*component)->getComp(j)->getVel(), true))) {
									if (field[(*iter).x][(*iter).y] == rc) {
										if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(left, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(down, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(up, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										prev.insert(pair<Point, Point>({ (*iter).x,(*iter).y }, (*component)->getCoord()));
									}
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
									(*component)->moveRobot(j, left, checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, left, (*component)->getComp(j)->getVel()));
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;

									prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
								}
								else if (((*iter).x > x) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, right, (*component)->getComp(j)->getVel(), true))) {
									if (field[(*iter).x][(*iter).y] == rc) {
										if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(right, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(down, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(up, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}

										prev.insert(pair<Point, Point>({ (*iter).x,(*iter).y }, (*component)->getCoord()));
									}
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
									(*component)->moveRobot(j, right, checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, right, (*component)->getComp(j)->getVel()));
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;
									prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
								}
								else if (((*iter).y < y) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, up, (*component)->getComp(j)->getVel(), true))) {
									if (field[(*iter).x][(*iter).y] == rc) {
										if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(left, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(right, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(up, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}

										prev.insert(pair<Point, Point>({ (*iter).x,(*iter).y }, (*component)->getCoord()));
									}
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
									(*component)->moveRobot(j, up, checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, up, (*component)->getComp(j)->getVel(), true));
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;
									prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
								}
								else if (((*iter).y > y) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, down, (*component)->getComp(j)->getVel(), true))) {
									if (field[(*iter).x][(*iter).y] == rc) {
										if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(left, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(right, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}
										else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel())) {
											field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
											dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(down, 1);
											field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
										}

										prev.insert(pair<Point, Point>({ (*iter).x,(*iter).y }, (*component)->getCoord()));
									}
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
									(*component)->moveRobot(j, down, checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, down, (*component)->getComp(j)->getVel()));
									field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;
									prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
								}
								break;
							}
						}
						if (!ways_left)
							visited_vertex[vertex.x][vertex.y] = V_visited;
					}
					if (visited_vertex[vertex.x][vertex.y] == V_visited) {
						_stacks[c_n][j].pop();
						TryClear = true;
					}

					if (_stacks[c_n][j].size() != 0) {
						if (_stacks[c_n][j].top() == nopoint) {
							break;
						}

						scanning = true;
						if (visited_vertex[vertex.x][vertex.y] == V_visited) {
							if ((_stacks[c_n][j].top().x > x) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, right, (*component)->getComp(j)->getVel(), true))) {
								if (field[_stacks[c_n][j].top().x][_stacks[c_n][j].top().y] == rc) {
									if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(right, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(down, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(up, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									prev.insert(pair<Point, Point>({ _stacks[c_n][j].top().x, _stacks[c_n][j].top().y }, (*component)->getCoord()));
								}
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
								(*component)->moveRobot(j, right, (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, right, (*component)->getComp(j)->getVel())));
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;
								prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
							}
							else if ((_stacks[c_n][j].top().x < x) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, left, (*component)->getComp(j)->getVel(), true))) {
								if (field[_stacks[c_n][j].top().x][_stacks[c_n][j].top().y] == rc) {
									if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(left, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(down, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(up, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									prev.insert(pair<Point, Point>({ _stacks[c_n][j].top().x, _stacks[c_n][j].top().y }, (*component)->getCoord()));
								}
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
								(*component)->moveRobot(j, left, (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, left, (*component)->getComp(j)->getVel())));
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;
								prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
							}
							else if ((_stacks[c_n][j].top().y > y) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, down, (*component)->getComp(j)->getVel(), true))) {
								if (field[_stacks[c_n][j].top().x][_stacks[c_n][j].top().y] == rc) {
									if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(left, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(right, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, down, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(down, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									prev.insert(pair<Point, Point>({ _stacks[c_n][j].top().x, _stacks[c_n][j].top().y }, (*component)->getCoord()));
								}
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
								(*component)->moveRobot(j, down, (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, down, (*component)->getComp(j)->getVel())));
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;
								prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
							}
							else if ((_stacks[c_n][j].top().y < y) && (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, up, (*component)->getComp(j)->getVel(), true))) {
								if (field[_stacks[c_n][j].top().x][_stacks[c_n][j].top().y] == rc) {
									if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, left, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(left, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, right, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(right, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									else if (checkMoveAbility(field, (*component)->getCoord().x, (*component)->getCoord().y, up, (*component)->getVel())) {
										field[(*component)->getCoord().x][(*component)->getCoord().y] = ai_seen;
										dynamic_cast<robotCommander*>((*component))->moveRobotInDirection(up, 1);
										field[(*component)->getCoord().x][(*component)->getCoord().y] = rc;
									}
									prev.insert(pair<Point, Point>({ _stacks[c_n][j].top().x, _stacks[c_n][j].top().y }, (*component)->getCoord()));
								}
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = ai_seen;
								(*component)->moveRobot(j, up, (checkMoveAbility(field, (*component)->getComp(j)->getCoord().x, (*component)->getComp(j)->getCoord().y, up, (*component)->getComp(j)->getVel())));
								field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rs;
								prev.insert(pair<Point, Point>({ x,y }, (*component)->getComp(j)->getCoord()));
							}
							//	std::cout << "\nHERE\n";
						}
					}
					else if (dead_robots.size() != 0) {
						scanning = true;
						_states[c_n].push(state_mode);
					}

					if (_stacks[c_n][j].size() == 0) {

						field[(*component)->getComp(j)->getCoord().x][(*component)->getComp(j)->getCoord().y] = rsd;
						_stacks[c_n][j].push({ -1, -1 });
					}
				}

				if (!scanning) {
					(*component)->setDead();
				}
				else {
					++component;
					++c_n;
				}
			}
			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime) {
				SDL_Delay(frameDelay - frameTime);
			}


			_render->init_text(_budget);
			_render->render_map(field, prev);


		}
		SDL_Delay(5000);
		_render->stop();
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
		int c_num = 0;
		while (fmc_it != components.end())
		{
			if ((*fmc_it)->iAm() == robot_commander)
				field[(*fmc_it)->getCoord().x][(*fmc_it)->getCoord().y] = rc;
			else
				field[(*fmc_it)->getCoord().x][(*fmc_it)->getCoord().y] = cc;

			_stacks.push_back(vector<stack<Point>>());
			for (size_t i = 0; i < (*fmc_it)->getNComp()->size(); i++)
			{
				_stacks[c_num].push_back(stack<Point>());
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
			++c_num;
		}

		std::cout << "\n Map created by robotic complex before scan:\n\n";
		showfield(field_Size, field, pointsOfInterest);

		auto begin = std::chrono::steady_clock::now();

		this->dfs(environment, field, components);

		auto end = std::chrono::steady_clock::now();


		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		std::cout << "\n TIME: " << elapsed_ms.count() << "ms";
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		std::cout << "\n Map created by robotic complex after scan:\n\n";
		showfield(field_Size, field, pointsOfInterest);

		std::cout << "\n\n Step: " << num << "\n\n";
		return pointsOfInterest;
	}
};