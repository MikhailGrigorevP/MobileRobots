#include <iostream>
#include <string>
#include "Module.h"

using std::string;
using std::vector;
using namespace Modules_N;

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//!Components namespace
/*! All components of robot complex:
 *
 * 1. Component (also Observe center) is basic class
 *
 * 2. managementComponent (also Command center) manage mobileComponent and robotScout
 *
 * 3. mobileComponent
 *
 * 4. robotScout
 *
 * 5. robotCommander
 * 
 */
namespace Components_N {

	//! Observe center, also Parent component
	class Component{
	private:
		string description;  //! Description
		int energy;  //! Energy of component
		int slotsNum; //! number of slots for modules
		vector<Module*> modules; //! array for modules 
		int cost; //! cost of component 
	protected:
		int x, y;  //! Coordinates
	public:

		vector<Module*>* getModules() {
			return &modules;
		}


		Component() :
			x(0),
			y(0),
			description("Observe center"),
			energy(0),
			slotsNum(0),
			cost(0) {

		};


		Module* getModule(int i) {
			return modules[i];
		}

		generatorModule* getGModule(int i) {
			return dynamic_cast<generatorModule*>(modules[i]);
		}

		sensorModule* getSModule(int i) {
			return dynamic_cast<sensorModule*>(modules[i]);
		}

		managementModule* getMModule(int i) {
			return dynamic_cast<managementModule*>(modules[i]);
		}
		int getModulesSize() {
			return modules.size();
		}

		Point getCoord() { return { x,y }; }
		string getDesc() { return description; }
		int getEnergy() { return energy; }
		int getNum() { return slotsNum; }
		int getCost() { return cost; }
		virtual int getNumD() { return 0; }
		virtual int getVel() { return 0; }

		Component(int x0, int y0, string desc, int en, int num, int c);

		Component(Point point, string desc, int en, int num, int c);

		virtual ~Component();

		virtual int iAm() const {return observe_center;}

		void setModule_g(int pr, int en, int c, int enpr);

		void setModule_s(int pr, int en, int c, int r, int ang, int direct);

		void setModule_m(int pr, int en, int c, int r, int n);

		void deleteModule(int type);

		void moduleOn(int type);

		void moduleOff(int type);
	};

	//! Command center
	/*! This virtual class is also "Command center"
	*
	* inheritor of "Component" with num of meneged devices
	*/
	class managementComponent : public virtual  Component 
	{
	private:
		int numOfDevices = 0;  //!< num of managed devices
		vector<Component*> managedComponents;  //!< array with managed devices
	public:
		managementComponent(int x0, int y0, int en, int num, int c, int numD, string desc);
		managementComponent(int numD = 0);


		vector<Component*>* getNComp() {
			return &managedComponents;
		} 

		Component* getComp(int i) {
			return managedComponents[i];
		}

		virtual ~managementComponent();

		virtual int getNumD() { return numOfDevices; }
		virtual int iAm() const { return command_center; }
		EnvironmentInfo getInfo(int i, ED_N::environmentDescriptor* env);

		virtual void moveRobot(int i, int direction);
	};

	//! Mobile component
	/*! This virtual class is mobile component
	*
	* inheritor of "Component" with velocity of managed devices
	*/
	class mobileComponent : public virtual  Component 
	{
	private:
		int velocity = 0;  //!< velocity of mobile component
	public:

		mobileComponent(int x0, int y0, string desc, int en = 0, int num = 0, int c = 0, int vel = 0);
		mobileComponent(int vel = 0);

		int getVel() { return velocity; }
		virtual ~mobileComponent();
		virtual int iAm() const = 0;
		virtual void moveRobotInDirection(int direction);
	};

	//! Robot scout - mobile component
	/*! This class is robot scout
	*
	* Inheritor of "mobileComponent"
	*
	* Robot scout can be moved and it observe environment
	*/
	class robotScout : public virtual mobileComponent {
	private:
	public:

		virtual int iAm() const { return robot_scout; }
		robotScout(int x0, int y0, string desc, int en, int num, int c, int vel = 0);
		~robotScout();
	};

	//! Robot commander - mobile and management component
	/*! This class is robot Commander
	*
	* Public inheritor of virtual "mobileComponent" and virtual "managementComponent"
	*
	* Robot commander can be moved and it observe environment, also it can manage components
	*/
	class robotCommander : public virtual mobileComponent, public virtual managementComponent
	{
	private:
	public:

		robotCommander(int x0, int y0, string desc, int en, int num, int c, int numD = 0, int vel = 0);
		virtual int iAm() const { return robot_commander; }
	    ~robotCommander();
	};

}
#endif