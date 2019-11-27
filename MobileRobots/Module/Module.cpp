#include "Module.h"
#include "../environmentDescriptor/environmentDescriptor.h"

#include <math.h>
#define PI 3.14159265

namespace Modules_N {
	//! turn module on
	void Module::on() {
		state = 1;
	};

	//! turn module off
	void Module::off() {
		state = 0;
	};

	//!get information from environment
	EnvironmentInfo sensorModule::getInfo(Point curr_location, ED_N::environmentDescriptor* env, vector<vector<unsigned>>& field) {
		EnvironmentInfo currInfo;

		if (!env)
			throw std::exception(" >>> incorrect environment");
		if ((curr_location.x < 0) || (curr_location.y < 0))
			throw std::exception(" >>> incorrect point");

		int y_start_point, x_start_point, x_start, x_end, y_start, y_end, curr_rad, dst;

		switch (direction) {
		case left:
			x_start_point = radius * cos(angle / 2 * PI / 180) < 0 ? round(radius * cos(angle / 2 * PI / 180)) : 0;
			x_end = -x_start_point + curr_location.x >= env->getSize().n ? env->getSize().n - 1 : -x_start_point + curr_location.x;
			x_start = -radius + curr_location.x < 0 ? 0 : -radius + curr_location.x;

			for (int i = x_start; i <= x_end; i++)
			{
				curr_rad = abs(i - curr_location.x);
				dst = round(curr_rad * tan(angle / 2 * PI / 180));
				y_start = (curr_location.y - dst) < 0 ? 0 : (curr_location.y - dst);
				y_end = (dst + curr_location.y) > (env->getSize().m - 1) ? (env->getSize().m - 1) : (dst + curr_location.y);
				for (int j = y_start; j <= y_end; j++)
				{
					if ((env->getCell({ i,j }) != notexist) && (i != curr_location.x || j != curr_location.y)) {
						if (env->getCell({ i,j }) == barrier) {
							currInfo.barriers.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = barrier;
						}
						else if (env->getCell({ i,j }) == interest_point) {
							currInfo.pointsOfInterest.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = interest_point;
						}
						else if ((field.size() != 2) && (field[i][j] == notexist)) {
							currInfo.allpoints.push_back({ i, j });
							field[i][j] = ai_seen;
						}

						vector<Components_N::Component*>* comp = env->getComponents();
						vector<Components_N::Component*>::iterator it;

						it = comp->begin();

						while (it != comp->end())
						{
							if ((i == (*it)->getCoord().x) && (j == (*it)->getCoord().y)) {
								if (field.size() != 2) {
									switch ((*it)->iAm()) {
									case robot_commander:
										field[i][j] = rc;
										break;
									case observe_center:
										field[i][j] = oc;
										break;
									case robot_scout:
										if (field[i][j]!=rsd)
											field[i][j] = rs;
										break;
									case command_center:
										field[i][j] = cc;
										break;
									}
								}
								currInfo.components.push_back(*it);
							}
							++it;
						}
					}
					else if ((env->getCell({ i,j }) == notexist) && (i != curr_location.x || j != curr_location.y)) {
						if ((field.size() != 2) && (field[i][j] == notexist)) {
							field[i][j] = border;
						}
					}
				}
			}
			break;
		case right:
			x_start_point = radius * cos(angle / 2 * PI / 180) < 0 ? round(radius * cos(angle / 2 * PI / 180)) : 0;
			x_start = x_start_point + curr_location.x < 0 ? 0 : x_start_point + curr_location.x;
			x_end = radius + curr_location.x >= env->getSize().n ? env->getSize().n - 1 : radius + curr_location.x;

			for (int i = x_start; i <= x_end; i++)
			{
				curr_rad = abs(i - curr_location.x);
				dst = round(curr_rad * tan(angle / 2 * PI / 180));
				y_start = (curr_location.y - dst) < 0 ? 0 : (curr_location.y - dst);
				y_end = (dst + curr_location.y) > (env->getSize().m - 1) ? (env->getSize().m - 1) : (dst + curr_location.y);
				for (int j = y_start; j <= y_end; j++)
				{
					if ((env->getCell({ i,j }) != notexist) && (i != curr_location.x || j != curr_location.y)) {
						if (env->getCell({ i,j }) == barrier) {
							currInfo.barriers.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = barrier;
						}
						else if (env->getCell({ i,j }) == interest_point) {
							currInfo.allpoints.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = interest_point;
						}
						else if ((field.size() != 2) && (field[i][j] == notexist)) {
							currInfo.allpoints.push_back({ i, j });
							field[i][j] = ai_seen;
						}

						vector<Components_N::Component*>* comp = env->getComponents();
						vector<Components_N::Component*>::iterator it;

						it = comp->begin();

						while (it != comp->end())
						{
							if ((i == (*it)->getCoord().x) && (j == (*it)->getCoord().y)) {
								if (field.size() != 2) {
									switch ((*it)->iAm()) {
									case robot_commander:
										field[i][j] = rc;
										break;
									case observe_center:
										field[i][j] = oc;
										break;
									case robot_scout:
										if (field[i][j] != rsd)
											field[i][j] = rs;
										break;
									case command_center:
										field[i][j] = cc;
										break;
									}
								}
								currInfo.components.push_back(*it);
							}
							++it;
						}
					}
					else if ((env->getCell({ i,j }) == notexist) && (i != curr_location.x || j != curr_location.y)) {
						if ((field.size() != 2) && (field[i][j] == notexist)) {
							field[i][j] = border;
						}
					}
				}
			}
			break;
		case up:
			y_start_point = radius * cos(angle / 2 * PI / 180) < 0 ? round(radius * cos(angle / 2 * PI / 180)) : 0;
			y_end = -y_start_point + curr_location.y >= env->getSize().m ? env->getSize().m - 1 : -y_start_point + curr_location.y;
			y_start = -radius + curr_location.y < 0 ? 0 : -radius + curr_location.y;

			for (int j = y_start; j <= y_end; j++)
			{
				curr_rad = abs(j - curr_location.y);
				dst = round(curr_rad * tan(angle / 2 * PI / 180));
				x_start = (-dst + curr_location.x) < 0 ? 0 : ceil(-dst + curr_location.x);
				x_end = (dst + curr_location.x) > (env->getSize().n - 1) ? (env->getSize().n- 1) : (dst + curr_location.x);
				for (int i = x_start; i <= x_end; i++)
				{
					if ((env->getCell({ i,j }) != notexist) && (i != curr_location.x || j != curr_location.y)) {
						if (env->getCell({ i,j }) == barrier) {
							currInfo.barriers.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = barrier;
						}
						else if (env->getCell({ i,j }) == interest_point) {
							currInfo.pointsOfInterest.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = interest_point;
						}
						else if ((field.size() != 2) && (field[i][j] == notexist)){
							currInfo.allpoints.push_back({ i, j });
							field[i][j] = ai_seen;
						}

						vector<Components_N::Component*>* comp = env->getComponents();
						vector<Components_N::Component*>::iterator it;

						it = comp->begin();

						while (it != comp->end())
						{
							if ((i == (*it)->getCoord().x) && (j == (*it)->getCoord().y)) {
								if (field.size() != 2) {
									switch ((*it)->iAm()) {
									case robot_commander:
										field[i][j] = rc;
										break;
									case observe_center:
										field[i][j] = oc;
										break;
									case robot_scout:
										if (field[i][j] != rsd)
											field[i][j] = rs;
										break;
									case command_center:
										field[i][j] = cc;
										break;
									}
								}
								currInfo.components.push_back(*it);
							}
							++it;
						}
					}
					else if ((env->getCell({ i,j }) == notexist) && (i != curr_location.x || j != curr_location.y)) {
						if ((field.size() != 2) && (field[i][j] == notexist)) {
							field[i][j] = border;
						}
					}
				}
			}
			break;
		case down:

			y_start_point = radius * cos(angle / 2 * PI / 180) < 0 ? round(radius * cos(angle / 2 * PI / 180)) : 0;
			y_start = y_start_point + curr_location.y < 0 ? 0 : y_start_point + curr_location.y;
			y_end = radius + curr_location.y >= env->getSize().m ? env->getSize().m- 1 : radius + curr_location.y;

			for (int j = y_start; j <= y_end; j++)
			{
				curr_rad = abs(j - curr_location.y);
				dst = round(curr_rad * tan(angle / 2 * PI / 180));
				x_start = (-dst + curr_location.x) < 0 ? 0 : (-dst + curr_location.x);
				x_end = (dst + curr_location.x) > (env->getSize().n - 1) ? (env->getSize().n- 1) : (dst + curr_location.x);

				for (int i = x_start; i <= x_end; i++)
				{
					if ((env->getCell({ i,j }) != notexist) && (i != curr_location.x || j != curr_location.y)) {
						if (env->getCell({ i,j }) == barrier) {
							currInfo.barriers.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = barrier;
						}
						else if (env->getCell({ i,j }) == interest_point) {
							currInfo.pointsOfInterest.push_back({ i, j });
							if (field.size() != 2)
								field[i][j] = interest_point;
						}
						else if ((field.size() != 2) && (field[i][j] == notexist)) {
							currInfo.allpoints.push_back({ i, j });
							field[i][j] = ai_seen;
						}

						vector<Components_N::Component*>* comp = env->getComponents();
						vector<Components_N::Component*>::iterator it;

						it = comp->begin();

						while (it != comp->end())
						{
							if ((i == (*it)->getCoord().x) && (j == (*it)->getCoord().y)) {
								if (field.size() != 2) {
									switch ((*it)->iAm()) {
									case robot_commander:
										field[i][j] = rc;
										break;
									case observe_center:
										field[i][j] = oc;
										break;
									case robot_scout:
										if (field[i][j] != rsd)
											field[i][j] = rs;
										break;
									case command_center:
										field[i][j] = cc;
										break;
									}
								}
								currInfo.components.push_back(*it);
							}
							++it;
						}
					}
					else if ((env->getCell({ i,j }) == notexist) && (i != curr_location.x || j != curr_location.y)) {
						if ((field.size() != 2) && (field[i][j] == notexist)) {
							field[i][j] = border;
						}
					}
				}
			}
			break;
			return {};
		}

		return currInfo;
	};

	//!< send management resourse
	void managementModule::sendResourse(Components_N::managementComponent* current, Components_N::Component* component) {
		if (current == component)
			throw std::exception(" >>> error");

		int x1 = current->getCoord().x;
		int x2 = component->getCoord().x;
		int y1 = current->getCoord().y;
		int y2 = component->getCoord().y;

		//if (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)) > radius)
		//	throw std::exception(" >>> component is too far");
		current->getNComp()->push_back(component);
		component->setActive();
	}

	//!< free management resourse
	void managementModule::freeResourse(Components_N::Component* current, int i) {
		if (i >= dynamic_cast<managementComponent*>(current)->getNComp()->size())
			throw std::exception(">>> incorrect num");

		dynamic_cast<managementComponent*>(current)->getNComp()->erase(dynamic_cast<managementComponent*>(current)->getNComp()->begin() + i);
	}
}