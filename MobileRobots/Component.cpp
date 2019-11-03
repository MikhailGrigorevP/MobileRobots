#include "Component.h"
#include <algorithm>
namespace Components_N {


	/*! Simple Constructor gets
	* x0 and y0 to set coordinates of module,
	* desc to set description,
	* en to set energy,
	* num to set slotsNNum,
	* c to set cost
	*/
	Component::Component(int x0, int y0, string desc, int en, int num, int c) :
		x(x0),
		y(y0),
		description(desc),
		energy(en),
		slotsNum(num),
		cost(c) {};

	/*! Simple Constructor gets
	* point to set coordinates of module,
	* desc to set description,
	* en to set energy,
	* num to set slotsNNum,
	* c to set cost
	*/
	Component::Component(Point point, string desc, int en, int num, int c) :
		x(point.x),
		y(point.y),
		description(desc),
		energy(en),
		slotsNum(num),
		cost(c) {};

	/*! virtual destructor delete dynamic array of component's modules
	*/
	Component::~Component() { modules.~vector(); };

	/*! Method to set module: gets type of module, check free slots and install module.
	*
	* Throw exception if out of free space
	*
	* Throw exception if you try to set management module on unmanegement component
	*/

	void Component::setModule_g(int pr, int en, int c, int enpr) {
		if(modules.size() == slotsNum)
			throw std::exception(" >>> not enough space for modules");
		if (pr < 0)
			throw std::exception(" >>> incorrect prioritet");
		if (en < 0)
			throw std::exception(" >>> incorrect energy level");
		if (c < 0)
			throw std::exception(" >>> incorrect cost");
		if (enpr < 0)
			throw std::exception(" >>> incorrect energy provision");

		Module* mod = new generatorModule(false, pr, en, c, enpr);
		modules.push_back(mod);
	};

	void Component::setModule_s(int pr, int en, int c, int r, int ang, int direct) {
		if (modules.size() == slotsNum)
			throw std::exception(" >>> not enough space for modules");
		if (pr < 0)
			throw std::exception(" >>> incorrect prioritet");
		if (en < 0)
			throw std::exception(" >>> incorrect energy level");
		if (c < 0)
			throw std::exception(" >>> incorrect cost");
		if (r < 0)
			throw std::exception(" >>> incorrect radius");
		if (ang < 0)
			throw std::exception(" >>> incorrect angle");
		if (direct < 0)
			throw std::exception(" >>> incorrect direction");

		Module* mod = new sensorModule(false, pr, en, c, r, ang, direct);
		modules.push_back(mod);
	};

	void Component::setModule_m(int pr, int en, int c, int r, int n) {
		if (modules.size() == slotsNum)
			throw std::exception(" >>> not enough space for modules");
		if (pr < 0)
			throw std::exception(" >>> incorrect prioritet");
		if (en < 0)
			throw std::exception(" >>> incorrect energy level");
		if (c < 0)
			throw std::exception(" >>> incorrect cost");
		if (r < 0)
			throw std::exception(" >>> incorrect radius");
		if (n < 0)
			throw std::exception(" >>> incorrect num of managed devices");
		if ((this->iAm() == robot_scout)||(this->iAm() == observe_center))
			throw std::exception(" >>> You can't set management module on unmanageble component");

		Module* mod = new managementModule(false, pr, en, c, r, n);
		modules.push_back(mod);
	
	};

	/*! Method to delete module: gets type of module, check availability of the module
	*
	*Throw exception if there is no module with this type
	*/
	void Component::deleteModule(int num) {
		if ((num < 0) || (num >= modules.size()))
			throw std::exception(" >>> incorrect num of module");
		modules.erase(modules.begin() + num);
	};

	/*! Method to turn module on: gets type of module, check availability of the module
	*
	* Throw exception if there is no module with this type
	*
	* Throw exception if module is already active
	*
	* It calls moduleOff in module
	*/
	void Component::moduleOn(int num) {
		if ((num < 0) || (num >= modules.size()))
			throw std::exception(" >>> incorrect num of module");
		modules[num]->on();
	};

	/*!  Method to turn module off: gets type of module, check availability of the module
	*
	* Throw exception if there is no module with this type
	*
	* Throw exception if module is already inactive
	*
	* It calls moduleOff in module
	*/
	void Component::moduleOff(int num) {
		if ((num < 0) || (num >= modules.size()))
			throw std::exception(" >>> incorrect num of module");
		modules[num]->off();
	};

	////////////////////////////////////////////////////////////
	//managementComponent
	////////////////////////////////////////////////////////////


	/*! Simple Constructor gets all variables for "Component" constructor and numD for numOfDevices */
	managementComponent:: managementComponent(int x0, int y0, int en, int num, int c, int numD, string desc) :
		Component(x0, y0, desc, en, num, c),
		numOfDevices(numD) {};
	/*! Constructor to set num of managed devices */
	managementComponent::managementComponent(int numD) : numOfDevices(numD) {
	};
	/*! virtual destructor to delete mananaged Components */
	managementComponent:: ~managementComponent() { managedComponents.~vector(); };

	/*! get environment information from robot
	*
	* throw exception, if there is no managed robot
	*/
	EnvironmentInfo managementComponent::getInfo(int i, ED_N::environmentDescriptor* env) {
		if (!env)
			throw std::exception(" >>> incorrect environment");
		if (i >= managedComponents.size())
			throw std::exception(">>> incorrect num");
		EnvironmentInfo EInf;
		std::vector<Modules_N::Module*>::iterator it = managedComponents[i]->getModules()->begin();

		while (it != managedComponents[i]->getModules()->end())
		{
			if ((*it)->iAm() == sensor_Module) {
				EnvironmentInfo EInf_old = dynamic_cast<sensorModule*>(*it)->getInfo(managedComponents[i]->getCoord(), env);

				std::vector<Point>::iterator iter;
				iter = EInf_old.barriers.begin();
				while (iter != EInf_old.barriers.end())
				{
					if (std::find(EInf.barriers.begin(), EInf.barriers.end(), *iter) == EInf.barriers.end()) {
						EInf.barriers.push_back(*iter);
					}
					++iter;
				}

				iter = EInf_old.pointsOfInterest.begin();
				while (iter != EInf_old.pointsOfInterest.end())
				{
					if (std::find(EInf.pointsOfInterest.begin(), EInf.pointsOfInterest.end(), *iter) == EInf.pointsOfInterest.end()) {
						EInf.pointsOfInterest.push_back(*iter);
					}
					++iter;
				}

				std::vector<Component*>::iterator iterc;
				iterc = EInf_old.components.begin();
				while (iterc != EInf_old.components.end())
				{
					if (std::find(EInf.components.begin(), EInf.components.end(), *iterc) == EInf.components.end()) {
						EInf.components.push_back(*iterc);
					}
					++iterc;
				}
			}
			++it;
		}
		return EInf;
	};
	/*! move robot
	*
	* throw exception, if there is no managed robot
	*
	* it calls moveRobotInDirection function from mobileComponent
	*/
	void managementComponent::moveRobot(int i, int direction) {
		if (i >= managedComponents.size())
			throw std::exception(">>> incorrect num");
		if ((direction < 0 )|| (direction > 3))
			throw std::exception(">>> incorrect direction");
		switch (managedComponents[i]->iAm()) {
		case robot_scout:
			(dynamic_cast<robotScout*>(managedComponents[i]))->moveRobotInDirection(direction);
			break;
		case robot_commander:
			(dynamic_cast<robotCommander*>(managedComponents[i]))->moveRobotInDirection(direction);
			break;
		default:
			throw std::exception(">>> static components can't move");
			break;
		}
	};

	////////////////////////////////////////////////////////////
	//mobileComponent
	////////////////////////////////////////////////////////////

	/*! Simple Constructor gets all variables for "Component" constructor and vel for velocity */
	mobileComponent:: mobileComponent(int x0, int y0, string desc, int en, int num, int c, int vel) :
		Component(x0, y0, desc, en, num, c),
		velocity(vel) {};
	/*! Constructor to set velocity of managed devices */
	mobileComponent::mobileComponent(int vel) : velocity(vel) {
	};
	/*! virtual destructor */
	mobileComponent::~mobileComponent() {
	};
	/*! move robot in given direction: throw exception, if there is no managed robot */
	void mobileComponent::moveRobotInDirection(int direction) {
		switch (direction)
		{
		case left:
			x -= velocity;
			break;
		case right:
			x += velocity;
			break;
		case down:
			y += velocity;
			break;
		case up:
			y -= velocity;
			break;
		default:
			throw std::exception(" >>> incorrect direction");
			break;
		}
	};

	////////////////////////////////////////////////////////////
	//robotScout
	////////////////////////////////////////////////////////////

	/*! Simple Constructor gets all variables for "Component" and vel for "mobileComponent" */
	robotScout::robotScout(int x0, int y0, string desc, int en, int num, int c, int vel) :
		Component(x0, y0, desc, en, num, c),
		mobileComponent(vel) {
	
	};
	/*! destructor */
	robotScout::~robotScout() {};

	////////////////////////////////////////////////////////////
	//robotCommander
	////////////////////////////////////////////////////////////

	/*! Simple Constructor gets all variables for "Component", vel for "mobileComponent", numD for "managementComponent" */
	robotCommander::robotCommander(int x0, int y0, string desc, int en, int num, int c, int numD, int vel) :
		Component(x0, y0, desc, en, num, c),
		mobileComponent(vel),
		managementComponent(numD) {
	};
	/*! destructor */
	robotCommander::~robotCommander() {};

}

