/*! \mainpage Mobile Robots
 *
 * This project helps to find points of interest on surface.
 * Currently two main things for robotic complex:
 * - \subpage componentsPage "What is robotic component?"
 *
 * - \subpage modulePage "What is component's module?"
 *
 * - \subpage howItWorkPage "How does it work?"
 *
 */

 /*! \page componentsPage What is robotic component?
  *
  * Coming soon
  *
  */

 /*! \page modulePage What is component's module?
  *
  * Coming soon
  *
  */


  /*! \page howItWorkPage How does it work
   *
   * This page is about how robotic complex work
   * Following sections describe robotic complex:
   * - \ref groupSearchingPoints ""
   * - \ref groupMovement "Movement"
   */

   /** \defgroup groupMovement Movement
	* Coming soon
	*/

#include "AI.h"
#include "environmentDescriptor.h"
#include <iostream>
#include <cstdlib>


//Main menu
const char* MENU_0[]{ "1. Work with Environment", "2. Work with components", "3. Show all information", "0. Exit" };
const unsigned menu_0_SZ = sizeof(MENU_0) / sizeof(MENU_0[0]);

//Environment menu
const char* MENU_1[]{ "1. Set field", "2. Get field size", "3. Set cell info", "4. Get cell info", "5. Add component to field", "0. <- Back to Main menu" };
const unsigned menu_1_SZ = sizeof(MENU_1) / sizeof(MENU_1[0]);

//Component menu
const char* MENU_2[]{ "1. Show all components", "2. Edit component's module", "3. Work with managed components", "4. Move robot", "0. <- Back to Main menu" };
const unsigned menu_2_SZ = sizeof(MENU_2) / sizeof(MENU_2[0]);


using namespace Components_N;
using namespace Modules_N;
using namespace AI_N;
using namespace ED_N;

void getClear()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//! Main
/*! Main
*
* 
*
*/
int main() {

	unsigned menu2 = 1;
	bool work2 = true;

	unsigned menu = 1;
	bool work = true;

	unsigned menu0 = 1;
	bool work0 = true;
	environmentDescriptor environment;
	do {
		if (menu0)
			for (size_t i = 0; i != menu_0_SZ; ++i)
				std::cout << MENU_0[i] << std::endl;

		std::cout << ">> ";
		std::cin >> menu0;
		if (std::cin.eof())
			work0 = false;

		switch (menu0)
		{
		case 0:
			work0 = false;
			break;
		case 1:
			//ENVIRONMENT
			menu = 1;
			std::cout << std::endl;
			do {
				if (menu)
					for (size_t i = 0; i != menu_1_SZ; ++i)
						std::cout << MENU_1[i] << std::endl;

				std::cout << ">> ";
				std::cin >> menu;
				if (std::cin.eof())
					work = false;

				Point point;
				unsigned type;
				int en, num, c, vel, numD;
				bool input;
				switch (menu)
				{
				case 0:
					work = false;
					if (system("CLS"))
						system("clear");
					break;
				case 1:
					std::cout << ">>> Input m and n for field" << std::endl;
					try {
						std::cin >> environment;
					}
					catch (std::exception & ex) {
						std::cout << ex.what() << std::endl;
					}
					break;
				case 2:
					try {
						std::cout << environment;
					}
					catch (std::exception & ex) {
						std::cout << ex.what() << std::endl;
					}
					break;
				case 3:
					std::cout << ">>> Input x and y of cell and cell's type (1 for barrier, 2 for interest_point) (-1 to stop or ctrl^z to exit)" << std::endl;

					input = true;
					do {
						std::cin >> point.x;
						if (point.x == -1) {
							std::cout << " >>> ended";
							break;
						}
						std::cin >> point.y >> type;
						if (std::cin.eof()) {
							std::cout << " >>> ended";
							break;
						}

						if (std::cin.fail()) {
							std::cout << " >>> input failed";
							break;
						}
						else {
							try {
								environment.setCell(point, type);
							}
							catch (std::exception & ex) {
								std::cout << ex.what() << std::endl;
							}
						}
					} while (input);
					break;
				case 4:
					std::cout << ">>> Input x and y of cell" << std::endl;
					std::cin >> point.x >> point.y;
					if (std::cin.fail()) {
						std::cout << "input failed";
						break;
					}
					try {
						type = environment.getCell(point);
						switch (type) {
						case none_cell:
							std::cout << "This is free cell";
							break;
						case barrier:
							std::cout << "This is barrier";
							break;
						case interest_point:
							std::cout << "This is interest's point";
							break;
						}
					}
					catch (std::exception & ex) {
						std::cout << ex.what() << std::endl;
					}
					break;
				case 5:
					std::cout << ">>> Input x, y, energy level, num of mudules, cost of component and component's type \n"
						<< "(0 for observecenter, 1 for command center, 2 for robot commander, 3 for robot scout) (-1 to stop or ctrl^z to exit)" << std::endl;

					input = true;
					do {
						std::cin >> point.x;
						if (point.x == -1) {
							std::cout << " >>> ended";
							break;
						}
						std::cin >> point.y >> en >> num >> c >> type;
						if (std::cin.eof()) {
							std::cout << " >>> ended";
							break;
						}

						if (std::cin.fail()) {
							std::cout << " >>> input failed";
							break;
						}

						switch (type) {
						case command_center: {
							std::cout << " >>> Input number of managed modules: ";
							std::cin >> numD;
							break; }
						case robot_commander: {
							std::cout << " >>> Input velovity and number of managed modules: ";
							std::cin >> vel;
							std::cin >> numD;
							break; }
						case robot_scout:
							std::cout << " >>> Input velocity modules: ";
							std::cin >> vel;
							break;
						}

						if (std::cin.fail()) {
							std::cout << " >>> input failed";
							break;
						}
						else {
							try {
								switch (type) {
								case observe_center:
									environment.AddObserveCenter(point, en, num, c);
									break;
								case command_center:
									environment.AddCommandCenter(point, en, num, numD, c);
									break;
								case robot_commander:
									environment.AddRobotCommander(point, en, num, numD, vel, c);
									break;
								case robot_scout:
									environment.AddRobotScout(point, en, num, vel, c);
									break;
								}
							}
							catch (std::exception & ex) {
								std::cout << ex.what() << std::endl;
							}
						}
					} while (input);
					break;
				default:
					std::cout << " >>> Enter error, try again" << std::endl;
					break;
				}

				if (menu) {
					std::cout << "\nEnter 0 to continue, 1 to show the menu once again (ctrl^Z to exit)" << std::endl;
					do {
						std::cout << ">> ";
						getClear();
						std::cin >> menu;
						if (std::cin.eof())
							work = false;
						else {
							if (!(std::cin.good()))
								std::cout << " >>> Enter error, try again" << std::endl;
						}
					} while ((work) && !(std::cin.good()));
				}

			} while (work);
			break;
		case 2:
			//COMPONENTS
			menu2 = 1;
			std::cout << std::endl;
			do {
				if (menu2)
					for (size_t i = 0; i != menu_2_SZ; ++i)
						std::cout << MENU_2[i] << std::endl;

				std::cout << ">> ";
				std::cin >> menu2;
				if (std::cin.eof())
					work2 = false;

				Point point;
				unsigned type;
				int cnum, mnum, ans, pr, en, c, enpr, r, ang, direct, n;
				bool input;
				switch (menu2) {

				case 0: {
					work2 = false;
					if (system("CLS"))
						system("clear");
					break;
				}
				case 1: {
					try {
						environment.showComponents();
					}
					catch (std::exception & ex) {
						std::cout << ex.what() << std::endl;
					}
					break; }
				case 2: {
					std::cout << ">>> Do you want to add or delete module? (0 or 1)" << std::endl;
					std::cin >> ans;
					if (std::cin.fail() || ((ans != 1) && (ans != 0))) {
						std::cout << " >>> wrong answer";
						break;
					}
					std::cout << ">>> Input component's num" << std::endl;
					std::cin >> cnum;
					if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
						std::cout << " >>> wrong component's num";
						break;
					}

					if (ans) {

						if (environment.getComponent(cnum)->getModulesSize() == 0) {
							std::cout << "\tNo modules in this components";
							break;
						}
						else {
							for (int j = 0; j < environment.getComponent(cnum)->getModulesSize(); ++j) {
								std::cout << "\t" << j << ". ";
								switch ((environment.getComponent(cnum)->getModule(j))->iAm()) {
								case generator_Module:
									std::cout << "Generator Module ";
									break;
								case sensor_Module:
									std::cout << "Sensor Module ";
									break;
								case management_Module:
									std::cout << "Management Module ";
									break;

								}
								std::cout << std::endl;
							}

							std::cout << ">>> Input module's num" << std::endl;
							std::cin >> mnum;
							environment.getComponent(cnum)->deleteModule(mnum);
						}
					}
					else {
						std::cout << ">>> Input module's type ( 0 for management_Module, 1 for generator_Module, 2 for sensor_Module)" << std::endl;
						std::cin >> mnum;
						if (std::cin.fail() || ((mnum != 0) && (mnum != 1) && (mnum != 2))) {
							std::cout << "wrong module's type";
							break;
						}

						if ((mnum == management_Module) && ((environment.getComponent(cnum)->iAm() == robot_scout) && (environment.getComponent(cnum)->iAm() == observe_center))) {
							std::cout << " >>> You can't set management module on unmanageble component";
							break;
						}

						std::cout << ">>> Input prioritet, energy and cost of module" << std::endl;

						std::cin >> pr >> en >> c;

						if (std::cin.fail()) {
							std::cout << " >>> input failed";
							break;
						}

						switch (mnum) {
						case generator_Module: {
							std::cout << " >>> Input energy provision: ";
							std::cin >> enpr;
							break; }
						case management_Module: {
							std::cout << " >>> Input radius and num of managed deviced: ";
							std::cin >> r;
							std::cin >> n;
							break; }
						case sensor_Module:
							std::cout << " >>> Input radius, angle and direction ";
							std::cin >> r >> ang >> direct;
							break;
						}
						if (std::cin.fail()) {
							std::cout << " >>> input failed";
							break;
						}

						switch (mnum) {
						case generator_Module: {
							environment.getComponent(cnum)->setModule_g(pr, en, c, enpr);
							break; }
						case sensor_Module: {
							environment.getComponent(cnum)->setModule_s(pr, en, c, r, ang, direct);
							break; }
						case management_Module:
							environment.getComponent(cnum)->setModule_m(pr, en, c, r, n);
							break;
						}
					}
					break; }
				case 3: {
					break; }
				case 4: {
					std::cout << ">>> Input component's num to move (NOTICE: you can move just robots)" << std::endl;
					std::cin >> cnum;

					if (std::cin.fail()) {
						std::cout << " >>> input failed";
						break;
					}

					std::cout << ">>> Input direction, where to move ( 0 - left, 1 - right, 2 - up, 3 - down)" << std::endl;
					std::cin >> type;

					if (std::cin.fail()) {
						std::cout << " >>> input failed";
						break;
					}

					if ((environment.getComponent(cnum)->iAm() != robot_scout) && (environment.getComponent(cnum)->iAm() != robot_commander)) {
						std::cout << " >>> you can't move static things";
						break;
					}

					bool moving = true;

					switch (type) {
					case right:
						if ((environment.getComponent(cnum)->getVel() + environment.getComponent(cnum)->getCoord().x) >= environment.getSize().n)
							moving = false;
						break;
					case left:
						if ((-environment.getComponent(cnum)->getVel() + environment.getComponent(cnum)->getCoord().x) < 0)
							moving = false;
						break;
					case down:
						if ((environment.getComponent(cnum)->getVel() + environment.getComponent(cnum)->getCoord().y) >= environment.getSize().m)
							moving = false;
						break;
					case up:
						if ((-environment.getComponent(cnum)->getVel() + environment.getComponent(cnum)->getCoord().y) < 0)
							moving = false;
						break;
					}
					if (!moving)
						std::cout << " >>> you can't step over field";
					else {
						switch (environment.getComponent(cnum)->iAm()) {
						case robot_scout:
							environment.getRSComponent(cnum)->moveRobotInDirection(type);
							break;
						case robot_commander:
							environment.getRCComponent(cnum)->moveRobotInDirection(type);
							break;
						}
					}
					break; }
				default:
					std::cout << " >>> Enter error, try again" << std::endl;
					break;
				}


				if (menu2) {
					std::cout << "\nEnter 0 to continue, 1 to show the menu once again (ctrl^Z to exit)" << std::endl;
					do {
						std::cout << ">> ";
						getClear();
						std::cin >> menu2;
						if (std::cin.eof())
							work2 = false;
						else {
							if (!(std::cin.good()))
								std::cout << " >>> Enter error, try again" << std::endl;
						}
					} while ((work2) && !(std::cin.good()));
				}

			} while (work2);
			break;
		case 3:
			try {
				environment.drawMap();
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}
			break;
		default:
			std::cout << " >>> Enter error, try again" << std::endl;
			break;
		}

	} while (work0);
	return 0;
}