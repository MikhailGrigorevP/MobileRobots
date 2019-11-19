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

#include "AI/AI.h"
#include "environmentDescriptor/environmentDescriptor.h"
#include <iostream>
#include <cstdlib>
#include "Render/render.h"
#include "SDL.h"
#include <functional>


using namespace Components_N;
using namespace Modules_N;
using namespace AI_N;
using namespace ED_N;


const char* MENU_0[]{ "1. Work with Environment", "2. Work with components", "3. Show all information", "4. Find points of interest", "0. Exit" };
const unsigned menu_0_SZ = sizeof(MENU_0) / sizeof(MENU_0[0]);
void WorkWithEnvironment(ED_N::environmentDescriptor& environment);
void WorkWithComponents(ED_N::environmentDescriptor& environment);
void ShowAllInformation(ED_N::environmentDescriptor& environment);
void FindInterestPoints(ED_N::environmentDescriptor& environment);
vector<std::function<void(ED_N::environmentDescriptor&)> > menu_1 = { WorkWithEnvironment, WorkWithComponents, ShowAllInformation, FindInterestPoints };

const char* MENU_1[]{ "1. Set field", "2. Get field size", "3. Set cell info", "4. Get cell info", "5. Add component to field", "0. <- Back to Main menu" };
const unsigned menu_1_SZ = sizeof(MENU_1) / sizeof(MENU_1[0]);
void SetField(ED_N::environmentDescriptor& environment);
void GetField(ED_N::environmentDescriptor& environment);
void SetCell(ED_N::environmentDescriptor& environment);
void GetCell(ED_N::environmentDescriptor& environment);
void AddComp(ED_N::environmentDescriptor& environment);
vector<std::function<void(ED_N::environmentDescriptor&)> > menu_0 = { SetField, GetField, SetCell, GetCell, AddComp };


const char* MENU_2[]{ "1. Show all components", "2. Add/delete component's module", "3. Work with managed components", "4. Move robot", "5. On/off component's module", "0. <- Back to Main menu" };
const unsigned menu_2_SZ = sizeof(MENU_2) / sizeof(MENU_2[0]);
void ShowAllComp(ED_N::environmentDescriptor& environment);
void ModuleEdit(ED_N::environmentDescriptor& environment);
void WorkWithMC(ED_N::environmentDescriptor& environment);
void Move(ED_N::environmentDescriptor& environment);
void ModuleTurn(ED_N::environmentDescriptor& environment);
vector<std::function<void(ED_N::environmentDescriptor&)> > menu_2 = { ShowAllComp, ModuleEdit, WorkWithMC, Move, ModuleTurn };

const char* MENU_3[]{ "1. Add resource", "2. Delete resource", "3. Move robot", "4. Get environment info", "0. <- Back to components menu" };
const unsigned menu_3_SZ = sizeof(MENU_3) / sizeof(MENU_3[0]);
void AddResourse(ED_N::environmentDescriptor& environment);
void DeleteResourse(ED_N::environmentDescriptor& environment);
void MoveRobot(ED_N::environmentDescriptor& environment);
void GetInfo(ED_N::environmentDescriptor& environment);
vector<std::function<void(ED_N::environmentDescriptor&)> > menu_3 = { AddResourse, DeleteResourse, MoveRobot, GetInfo };

void getClear()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#include <fstream>
using std::ifstream;
void readFromFile (ED_N::environmentDescriptor& environment)
{
	ifstream file("map.txt");
	string str;
	int i = 0;
	int j = 0;
	int m = 0;
	int n = 0;
	while (!file.eof()) {
		m++;
		getline(file, str);
		if(str.size())
			n = str.size();
	}
	file.seekg(0);
	environment.setSize({ m - 1, n });
	//vbstd::cout << n << " " << m << std::endl;
	ifstream file2("map.txt");
	while (getline(file2, str)) {
		i = 0;
		for (char& c : str) {
			switch (c) {
			case '-':
				environment.setCell({ i, j }, none_cell);
				break;
			case '*':
				environment.setCell({ i, j }, interest_point);
				break;
			case '#':
				environment.setCell({ i, j }, barrier);
				break;
			}
			++i;
		}
		++j;
	}
}


//Init
void menu_InitEnvironment(ED_N::environmentDescriptor& environment) {
	environment.setSize({ 16, 16 });
	environment.setCell({ 0, 0 }, 2);
	environment.setCell({ 0, 5 }, 1);
	environment.setCell({ 0, 6 }, 1);
	environment.setCell({ 2, 1 }, 1);
	environment.setCell({ 4, 1 }, 1);
	environment.setCell({ 4, 2 }, 1);
	environment.setCell({ 5, 1 }, 1);
	environment.setCell({ 10, 1 }, 2);
	environment.setCell({ 12, 1 }, 1);
	environment.setCell({ 7, 3 }, 2);
	environment.setCell({ 2, 4 }, 1);
	environment.setCell({ 13, 5 }, 1);
	environment.setCell({ 3, 7 }, 1);
	environment.setCell({ 14, 8 }, 2);
	environment.setCell({ 5, 9 }, 1);
	environment.setCell({ 10, 9 }, 1);
	environment.setCell({ 1, 11 }, 1);
	environment.setCell({ 7, 11 }, 2);
	environment.setCell({ 5, 13 }, 2);
	environment.setCell({ 9, 14 }, 1);
	environment.setCell({ 14, 14 }, 1);

	////0
	//environment.AddCommandCenter({ 8, 8 }, 150, 4, 7, 1000);
	//environment.getCCComponent(0)->setModule_g(1, 100, 250, 9000);
	//environment.getCCComponent(0)->setModule_m(2, 150, 250, 9, 10);
	//1
	environment.AddRobotScout({ 5, 2 }, 80, 4, 2, 600);
	environment.getComponent(0)->setModule_g(1, 100, 125, 4500);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 0);
	environment.getComponent(0)->moduleOn(1);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 1);
	environment.getComponent(0)->moduleOn(2);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 2);
	////2
	//environment.AddRobotScout({ 1, 6 }, 80, 5, 2, 600);
	//environment.getComponent(2)->setModule_g(1, 100, 125, 4500);
	//environment.getRSComponent(2)->setModule_s(1, 150, 500, 4, 60, 0);
	//environment.getComponent(2)->moduleOn(1);
	//environment.getRSComponent(2)->setModule_s(1, 150, 500, 4, 60, 1);
	//environment.getRSComponent(2)->setModule_s(1, 150, 500, 4, 60, 2);
	////3
	//environment.AddRobotScout({ 12, 7 }, 80, 5, 2, 600);
	//environment.getComponent(3)->setModule_g(1, 100, 125, 4500);
	//environment.getRSComponent(3)->setModule_s(1, 150, 500, 4, 60, 3);
	//environment.getRSComponent(3)->setModule_s(1, 150, 500, 4, 60, 1);
	//environment.getRSComponent(3)->setModule_s(1, 150, 500, 4, 60, 2);

	////4
	//environment.AddRobotScout({ 2, 13 }, 80, 5, 2, 600);
	//environment.getComponent(4)->setModule_g(1, 100, 125, 4500);
	//environment.getRSComponent(4)->setModule_s(1, 150, 500, 4, 60, 3);
	//environment.getRSComponent(4)->setModule_s(1, 150, 500, 4, 60, 1);
	//environment.getComponent(4)->setModule_g(1, 100, 125, 6000);
	//5
	environment.AddRobotCommander({ 1, 2 }, 150, 4, 2, 1, 600);
	environment.getComponent(1)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(1)->setModule_m(2, 150, 250, 9, 4);
	dynamic_cast<managementComponent*>(environment.getComponent(1))->getMModule(1)->sendResourse(environment.getRCComponent(1), environment.getComponent(0));

	environment.AddRobotScout({ 14, 12 }, 80, 4, 2, 600);
	environment.getComponent(2)->setModule_g(1, 100, 125, 4500);
	environment.getRSComponent(2)->setModule_s(1, 150, 500, 4, 60, 0);
	environment.getComponent(2)->moduleOn(1);
	environment.getRSComponent(2)->setModule_s(1, 150, 500, 4, 60, 1);
	environment.getComponent(2)->moduleOn(2);
	environment.getRSComponent(2)->setModule_s(1, 150, 500, 4, 60, 2);

	environment.AddRobotCommander({ 12, 10 }, 150, 4, 2, 1, 600);
	environment.getComponent(3)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(3)->setModule_m(2, 150, 250, 9, 4);
	dynamic_cast<managementComponent*>(environment.getComponent(3))->getMModule(1)->sendResourse(environment.getRCComponent(3), environment.getComponent(2));

	//dynamic_cast<managementComponent*>(environment.getComponent(1))->getMModule(1)->sendResourse(environment.getRCComponent(1), environment.getComponent(2));
	////6
	//environment.AddObserveCenter({ 13, 2 }, 80, 5, 600);
	//environment.getComponent(6)->setModule_g(1, 100, 125, 4500);
	//environment.getComponent(6)->setModule_s(1, 150, 500, 4, 60, 0);
	//environment.getComponent(6)->setModule_s(1, 150, 500, 4, 60, 1);
	//environment.getComponent(6)->setModule_s(1, 150, 500, 4, 60, 2);
	//environment.getComponent(6)->setModule_s(1, 150, 500, 4, 60, 3);
	////7
	//environment.AddObserveCenter({ 12, 14 }, 80, 5, 600);
	//environment.getComponent(7)->setModule_g(1, 100, 125, 4500);

	//////////////////////////////////////
	//////////////////////////////////////

};
void menu_InitEnvironment_2(ED_N::environmentDescriptor& environment) {

	readFromFile(environment);

	environment.AddRobotScout({ 5, 3 }, 80, 5, 1, 600);
	environment.getComponent(0)->setModule_g(1, 100, 125, 4500);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 1, 60, 0);
	environment.getComponent(0)->moduleOn(1);

	environment.AddRobotCommander({ 5, 5 }, 150, 4, 2, 1, 600);
	environment.getComponent(1)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(1)->setModule_m(2, 150, 250, 3, 4);

	environment.AddRobotScout({ 3, 5 }, 80, 5, 1, 600);
	environment.getComponent(2)->setModule_g(1, 100, 125, 4500);
	environment.getRSComponent(2)->setModule_s(1, 150, 500, 1, 60, 2);
	environment.getComponent(2)->moduleOn(1);

	environment.AddObserveCenter({ 4, 7}, 80, 5, 600);
	environment.getComponent(3)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(3)->setModule_s(1, 150, 500, 4, 60, 0);
	environment.getComponent(3)->setModule_s(1, 150, 500, 4, 60, 1);
	environment.getComponent(3)->setModule_s(1, 150, 500, 4, 60, 2);
	environment.getComponent(3)->setModule_s(1, 150, 500, 4, 60, 3);
	environment.getComponent(3)->moduleOn(1);
	environment.getComponent(3)->moduleOn(2);
	environment.getComponent(3)->moduleOn(3);
	environment.getComponent(3)->moduleOn(4);

	dynamic_cast<managementComponent*>(environment.getComponent(1))->getMModule(1)->sendResourse(environment.getRCComponent(1), environment.getComponent(0));
	dynamic_cast<managementComponent*>(environment.getComponent(1))->getMModule(1)->sendResourse(environment.getRCComponent(1), environment.getComponent(2));


};

//Menu 0
void WorkWithEnvironment(ED_N::environmentDescriptor& environment) {
	unsigned menu = 1;
	bool work = true;	
	if (system("CLS"))
		system("clear");

	do {
		if (menu)
			for (size_t i = 0; i != menu_1_SZ; ++i)
				std::cout << MENU_1[i] << std::endl;

		std::cout << ">> "; std::cin >> menu;

		if (std::cin.eof()) {
			std::cout << " >>> End Of File";
			work = false;
		}
		else if (!menu) {
			return;
		}
		else
			menu_0[menu - 1](environment);

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
}
void WorkWithComponents(ED_N::environmentDescriptor& environment) {

	unsigned menu = 1;
	bool work = true;
	if (system("CLS"))
		system("clear");

	do {
		if (menu)
			for (size_t i = 0; i != menu_1_SZ; ++i)
				std::cout << MENU_2[i] << std::endl;

		std::cout << ">> "; std::cin >> menu;

		if (std::cin.eof()) {
			std::cout << " >>> End Of File";
			work = false;
		}
		else if (!menu) {
			return;
		}
		else
			menu_2[menu - 1](environment);

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

};
void ShowAllInformation(ED_N::environmentDescriptor& environment) {
	try {
		environment.drawMap();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
	return;
};
void FindInterestPoints(ED_N::environmentDescriptor& environment) {
	AI _AI;

	vector<Point> pointsOfInterest;
	pointsOfInterest = _AI.findInterestPoints(&environment);

	//render* _render = new render("NameOfGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, false);

	vector<Point>::iterator p_it;

	std::cout << "Points of interest: ";

	if (pointsOfInterest.size() != 0) {
		p_it = pointsOfInterest.begin();
		while (p_it != pointsOfInterest.end())
		{
			std::cout << "(" << (*p_it).x << ";" << (*p_it).y << ") ";
			++p_it;
		}
	}
	else
		std::cout << "points weren't found";
	std::cout << std::endl;
	std::cout << std::endl;
	return;
};

//Menu 1
void SetField(ED_N::environmentDescriptor& environment) {
	std::cout << ">>> Input m and n for field" << std::endl;
	try {
		std::cin >> environment;
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}
void GetField(ED_N::environmentDescriptor& environment) {
	try {
		std::cout << environment;
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}
void SetCell(ED_N::environmentDescriptor& environment) {

	Point point;
	bool input;
	int type;

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
}
void GetCell(ED_N::environmentDescriptor& environment) {

	Point point;
	int type;

	std::cout << ">>> Input x and y of cell" << std::endl;
	std::cin >> point.x >> point.y;
	if (std::cin.fail()) {
		std::cout << "input failed";
		return;
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
}
void AddComp(ED_N::environmentDescriptor& environment) {
	Point point;
	unsigned type;
	int en, num, c, vel, numD;
	bool input;

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
		case robot_scout: {
			std::cout << " >>> Input velocity modules: ";
			std::cin >> vel;
			break;		}
		case observe_center: {
			break; }
		default: {
			std::cout << " >>> Wrong type\n";
			break; }
		}

		if (std::cin.fail()) {
			std::cout << " >>> input failed";
			break;
		}
		else {
			switch (type) {
			case observe_center:
				try {
					environment.AddObserveCenter(point, en, num, c);
				}
				catch (std::exception & ex) {
					std::cout << ex.what() << std::endl;
				}
				break;
			case command_center:
				try {
					environment.AddCommandCenter(point, en, num, numD, c);
				}
				catch (std::exception & ex) {
					std::cout << ex.what() << std::endl;
				}
				break;
			case robot_commander:
				try {
					environment.AddRobotCommander(point, en, num, numD, vel, c);
				}
				catch (std::exception & ex) {
					std::cout << ex.what() << std::endl;
				}
				break;
			case robot_scout:
				try {
					environment.AddRobotScout(point, en, num, vel, c);
				}
				catch (std::exception & ex) {
					std::cout << ex.what() << std::endl;
				}
				break;
			}
		}
	} while (input);
}

//Menu 2
void ShowAllComp(ED_N::environmentDescriptor& environment) {
	try {
		environment.showComponents();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}
void ModuleEdit(ED_N::environmentDescriptor& environment) {
	int cnum, mnum, ans, pr, en, c, enpr, r, ang, direct, n;
	std::cout << ">>> Do you want to add or delete module? (0 or 1)" << std::endl;
	std::cin >> ans;
	if (std::cin.fail() || ((ans != 1) && (ans != 0))) {
		std::cout << " >>> wrong answer";
		return;
	}
	try {
		environment.showComponents();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
	std::cout << ">>> Input component's num" << std::endl;
	std::cin >> cnum;
	if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
		std::cout << " >>> wrong component's num";
		return;
	}

	if (ans) {

		if (environment.getComponent(cnum)->getModulesSize() == 0) {
			std::cout << "\tNo modules in this components";
			return;
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
			try {
				environment.getComponent(cnum)->deleteModule(mnum);
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}
		}
	}
	else {
		std::cout << ">>> Input module's type ( 0 for management_Module, 1 for generator_Module, 2 for sensor_Module)" << std::endl;
		std::cin >> mnum;
		if (std::cin.fail() || ((mnum != 0) && (mnum != 1) && (mnum != 2))) {
			std::cout << "wrong module's type";
			return;
		}

		if ((mnum == management_Module) && ((environment.getComponent(cnum)->iAm() == robot_scout) || (environment.getComponent(cnum)->iAm() == observe_center))) {
			std::cout << " >>> You can't set management module on unmanageble component";
			return;
		}

		std::cout << ">>> Input prioritet, energy and cost of module" << std::endl;

		std::cin >> pr >> en >> c;

		if (std::cin.fail()) {
			std::cout << " >>> input failed";
			return;
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
			return;
		}

		switch (mnum) {
		case generator_Module: {
			try {
				environment.getComponent(cnum)->setModule_g(pr, en, c, enpr);
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}
			break; }
		case sensor_Module: {
			try {
				environment.getComponent(cnum)->setModule_s(pr, en, c, r, ang, direct);
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}
			break; }
		case management_Module: {
			try {
				environment.getComponent(cnum)->setModule_m(pr, en, c, r, n);
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}
			break; }

		}
	}
}
void WorkWithMC(ED_N::environmentDescriptor& environment) {

	unsigned menu = 1;
	bool work = true;
	if (system("CLS"))
		system("clear");

	do {
		if (menu)
			for (size_t i = 0; i != menu_3_SZ; ++i)
				std::cout << MENU_3[i] << std::endl;

		std::cout << ">> "; std::cin >> menu;

		if (std::cin.eof()) {
			std::cout << " >>> End Of File";
			work = false;
		}
		else if (!menu) {
			return;
		}
		else
			menu_3[menu - 1](environment);

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

}
void Move(ED_N::environmentDescriptor& environment) {
	int cnum, type;
	std::cout << std::endl;
	std::cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
	std::cout << " >>> WARNING. You can’t make the object move itself. JUST FOR TEST >>>" << std::endl;
	std::cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
	std::cout << std::endl;
	try {
		environment.showComponentsNM();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}

	std::cout << ">>> Input component's num to move (NOTICE: you can move just robots)" << std::endl;
	std::cin >> cnum;

	if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	std::cout << ">>> Input direction, where to move ( 0 - left, 1 - right, 2 - up, 3 - down)" << std::endl;
	std::cin >> type;

	if (std::cin.fail()) {
		std::cout << " >>> input failed";
		return;
	}

	if ((environment.getComponent(cnum)->iAm() != robot_scout) && (environment.getComponent(cnum)->iAm() != robot_commander)) {
		std::cout << " >>> you can't move static things";
		return;
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
			try {
				environment.getRSComponent(cnum)->moveRobotInDirection(type);
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}
			break;
		case robot_commander:
			try {
				environment.getRCComponent(cnum)->moveRobotInDirection(type);
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}
			break;
		}
	}
}
void ModuleTurn(ED_N::environmentDescriptor& environment) {
	int ans, cnum;
	try {
		environment.showComponentsNM();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}

	std::cout << ">>> Input component's num to manage" << std::endl;
	std::cin >> cnum;

	if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	if (environment.getComponent(cnum)->getModulesSize() == 0) {
		std::cout << "\tNo modules in this components";
		return;
	}
	else {
		for (int j = 0; j < environment.getComponent(cnum)->getModulesSize(); ++j) {
			std::cout << "\t" << j << ". ";

			switch ((environment.getComponent(cnum)->getModule(j))->iAm()) {
			case generator_Module:
				std::cout << "Generator Module, state: " << environment.getComponent(cnum)->getModule(j)->getState();
				break;
			case sensor_Module:
				std::cout << "Sensor Module, state: " << environment.getComponent(cnum)->getModule(j)->getState();
				break;
			case management_Module:
				std::cout << "Management Module, state: " << environment.getComponent(cnum)->getModule(j)->getState();
				break;

			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << ">>> Input num of module to change it state" << std::endl;
	std::cin >> ans;

	if (std::cin.fail() || (ans >= environment.getComponent(cnum)->getModulesSize()) || (ans < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	try {
		if (environment.getComponent(cnum)->getModule(ans)->getState() == 0) {
			EnvironmentInfo EInf;
			vector<Modules_N::Module*>::iterator it = environment.getComponent(cnum)->getModules()->begin();

			int enProv = 0;

			while (it != environment.getComponent(cnum)->getModules()->end())
			{
				if (((*it)->iAm() == generator_Module) && ((*it)->getState() == 1)) {
					enProv += dynamic_cast<generatorModule*>((*it))->getEnergyProvision();
				}
				++it;
			}
			if (enProv > environment.getComponent(cnum)->getModule(ans)->getEnergy() + environment.getComponent(cnum)->getEnergy())
				environment.getComponent(cnum)->moduleOn(ans);
			else
				std::cout << " >>> component doesn't have enough energy";
		}
		else {
			environment.getComponent(cnum)->moduleOff(ans);
		}

	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}

//Menu 3
void AddResourse(ED_N::environmentDescriptor& environment){
	int mnum, cnum, ans;
	try {
		environment.showComponentsNM();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}

	std::cout << ">>> Input component's num to manage (NOTICE: you can manage just robot - commander or comand center)" << std::endl;
	std::cin >> cnum;

	if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}


	if ((environment.getComponent(cnum)->iAm() != robot_commander) && (environment.getComponent(cnum)->iAm() != command_center)) {
		std::cout << " >>> you can't manage unmanageble things";
		return;
	}


	if (environment.getComponent(cnum)->getModulesSize() == 0) {
		std::cout << "\tNo modules in this components";
		return;
	}
	else {
		for (int j = 0; j < environment.getComponent(cnum)->getModulesSize(); ++j) {
			std::cout << "\t" << j << ". ";

			switch ((environment.getComponent(cnum)->getModule(j))->iAm()) {
			case generator_Module:
				std::cout << "Generator Module";
				break;
			case sensor_Module:
				std::cout << "Sensor Module";
				break;
			case management_Module:
				std::cout << "Management Module";
				break;

			}
		}
		std::cout << std::endl;
	}

	std::cout << ">>> Input num of Management module if exist" << std::endl;
	std::cin >> ans;

	if (std::cin.fail() || (ans >= environment.getComponent(cnum)->getModulesSize()) || (ans < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	if (environment.getComponent(cnum)->getModule(ans)->iAm() != management_Module) {
		std::cout << " >>> module is not management";
		return;
	}
	if (environment.getComponent(cnum)->getMModule(ans)->getState() == 0) {
		std::cout << " >>> module is switched off";
		return;
	}

	if ((environment.getComponent(cnum)->getMModule(ans)->iAm() != management_Module)) {
		std::cout << " >>> you can't manage unmanageble things";
		return;
	}

	std::cout << ">>> Input component's num to add" << std::endl;
	std::cin >> mnum;

	if (std::cin.fail() || (mnum >= environment.getCompCount()) || (mnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	try {
		dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getMModule(ans)->sendResourse(dynamic_cast<managementComponent*>(environment.getComponent(cnum)), environment.getComponent(mnum));
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}
void DeleteResourse(ED_N::environmentDescriptor& environment) {
	int cnum, mnum, ans;

	try {
		environment.showComponentsNM();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}

	std::cout << ">>> Input component's num to manage (NOTICE: you can manage just robot - commander or comand center)" << std::endl;
	std::cin >> cnum;

	if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}


	if ((environment.getComponent(cnum)->iAm() != robot_commander) && (environment.getComponent(cnum)->iAm() != command_center)) {
		std::cout << " >>> you can't manage unmanageble things";
		return;
	}


	if (environment.getComponent(cnum)->getModulesSize() == 0) {
		std::cout << "\tNo modules in this components";
		return;
	}
	else {
		for (int j = 0; j < environment.getComponent(cnum)->getModulesSize(); ++j) {
			std::cout << "\t" << j << ". ";

			switch ((environment.getComponent(cnum)->getModule(j))->iAm()) {
			case generator_Module:
				std::cout << "Generator Module";
				break;
			case sensor_Module:
				std::cout << "Sensor Module";
				break;
			case management_Module:
				std::cout << "Management Module";
				break;

			}
		}
		std::cout << std::endl;
	}

	std::cout << ">>> Input num of Management module if exist" << std::endl;
	std::cin >> ans;

	if (std::cin.fail() || (ans >= environment.getComponent(cnum)->getModulesSize()) || (ans < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	if (environment.getComponent(cnum)->getModule(ans)->iAm() != management_Module) {
		std::cout << " >>> module is not management";
		return;
	}


	if (environment.getComponent(cnum)->getMModule(ans)->getState() == 0) {
		std::cout << " >>> module is switched off";
		return;
	}


	if ((environment.getComponent(cnum)->getMModule(ans)->iAm() != management_Module)) {
		std::cout << " >>> you can't manage unmanageble things";
		return;
	}


	vector<Components_N::Component*>::iterator c_it;
	std::cout << "\tManaged components: ";
	if (dynamic_cast<robotCommander*>(environment.getComponent(cnum))->getNComp()->size() != 0) {
		c_it = dynamic_cast<robotCommander*>(environment.getComponent(cnum))->getNComp()->begin();
		while (c_it != dynamic_cast<robotCommander*>(environment.getComponent(cnum))->getNComp()->end())
		{
			std::cout << "(" << (*c_it)->getCoord().x << ";" << (*c_it)->getCoord().y << ") ";
			++c_it;
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "components weren't found";
		return;
	}

	std::cout << ">>> Input component's num to delete" << std::endl;
	std::cin >> mnum;

	if (std::cin.fail() || (mnum >= environment.getCompCount()) || (mnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	try {
		dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getMModule(ans)->freeResourse(dynamic_cast<managementComponent*>(environment.getComponent(cnum)), mnum);
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}
void MoveRobot(ED_N::environmentDescriptor& environment) {
	int cnum, mnum, type;
	bool pr;

	try {
		environment.showComponentsNM();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}

	std::cout << ">>> Input component's num to manage (NOTICE: you can manage just robot - commander or comand center)" << std::endl;
	std::cin >> cnum;
	if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}


	if ((environment.getComponent(cnum)->iAm() != robot_commander) && (environment.getComponent(cnum)->iAm() != command_center)) {
		std::cout << " >>> you can't manage unmanageble things";
		return;
	}

	vector<Components_N::Component*>::iterator c_it;
	std::cout << "\tManaged components: ";
	if (dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getNComp()->size() != 0) {
		c_it = dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getNComp()->begin();
		while (c_it != dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getNComp()->end())
		{
			std::cout << "(" << (*c_it)->getCoord().x << ";" << (*c_it)->getCoord().y << ") ";
			++c_it;
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "components weren't found";
		return;
	}

	std::cout << ">>> Input component's num to move (NOTICE: you can move just robots)" << std::endl;
	std::cin >> mnum;

	if (std::cin.fail() || (mnum >= environment.getCompCount()) || (mnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	if ((dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->iAm() != robot_scout) && (dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->iAm() != robot_commander)) {
		std::cout << " >>> you can't move static things";
		return;
	}


	std::cout << ">>> Input direction, where to move ( 0 - left, 1 - right, 2 - up, 3 - down)" << std::endl;
	std::cin >> type;


	pr = true;
	switch (type) {
	case right:
		if ((dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getVel() + dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getCoord().x) >= environment.getSize().n)
			pr = false;
		break;
	case left:
		if ((-dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getVel() + dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getCoord().x) < 0)
			pr = false;
		break;
	case down:
		if ((dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getVel() + dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getCoord().y) >= environment.getSize().m)
			pr = false;
		break;
	case up:
		if ((-dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getVel() + dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getComp(mnum)->getCoord().y) < 0)
			pr = false;
		break;
	}
	if (!pr) {
		std::cout << " >>> you can't move over map";
		return;
	}

	try {
		dynamic_cast<managementComponent*>(environment.getComponent(cnum))->moveRobot(mnum, type);
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}
void GetInfo(ED_N::environmentDescriptor& environment) {
	int cnum, mnum;
	try {
		environment.showComponentsNM();
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}

	std::cout << ">>> Input component's num to manage (NOTICE: you can manage just robot - commander or comand center)" << std::endl;
	std::cin >> cnum;
	if (std::cin.fail() || (cnum >= environment.getCompCount()) || (cnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	if ((environment.getComponent(cnum)->iAm() != robot_commander) && (environment.getComponent(cnum)->iAm() != command_center)) {
		std::cout << " >>> you can't manage unmanageble things";
		return;
	}

	vector<Components_N::Component*>::iterator c_it;
	std::cout << "\tManaged components: ";
	if (dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getNComp()->size() != 0) {
		c_it = dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getNComp()->begin();
		while (c_it != dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getNComp()->end())
		{
			std::cout << "(" << (*c_it)->getCoord().x << ";" << (*c_it)->getCoord().y << ") ";
			++c_it;
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "components weren't found";
		return;
	}

	std::cout << ">>> Input component's num to get info from" << std::endl;
	std::cin >> mnum;

	if (std::cin.fail() || (mnum >= environment.getCompCount()) || (mnum < 0)) {
		std::cout << " >>> input failed";
		return;
	}

	try {
		environment.showInfo(dynamic_cast<managementComponent*>(environment.getComponent(cnum))->getInfo(mnum, &environment));
	}
	catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
	}
}



//! Main
/*! Main
*
* 
*
*/


int main(int argc, char* args[]) {

	unsigned menu = 1;
	bool work = true;

	environmentDescriptor environment;

	//menu_InitEnvironment(environment);
	menu_InitEnvironment_2(environment);

	do {

		if (menu)
			for (size_t i = 0; i != menu_0_SZ; ++i)
				std::cout << MENU_0[i] << std::endl;

		std::cout << ">> "; std::cin >> menu;

		if (std::cin.eof()) {
			std::cout << " >>> End Of File";
			work = false;
		}
		else if (!menu) {
			std::cout << " >>> Ended";
			work = false;
		}
		else
			menu_1[menu - 1](environment);

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

	return 0;
}