#include <iostream>
#include <string>
#include "Module.h"

using std::string;

#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include "Component.h"
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
		int x, y;  //!< Coordinates
		string description;  //!< Description
		int energy;  //!< Energy of component
		const int slotsNum; //!< number of slots for modules
		Modules_N::Module* modules; //!< array for modules 
		int cost; //!< cost of component 
	public:
		/*! Simple Constructor gets
		* x0 and y0 to set coordinates of module, 
		* desc to set description, 
		* en to set energy, 
		* num to set slotsNNum, 
		* c to set cost
		*/
		Component(int x0 = 0, int y0 = 0, string desc = "Observe center", int en = 0, int num = 0, int c = 0):
			x(x0), 
			y(y0), 
			description(desc),
			energy(en), 
			slotsNum(num), 
			cost(c) {};
		/*! virtual destructor delete dynamic array of component's modules
		*/
		virtual ~Component() { delete[] modules; };
		/*! Method to set module: gets type of module, check free slots and install module. 
		*
		* Throw exception if out of free space
		*
		* Throw exception if you try to set management module on unmanegement component
		*/
		void setModule(int type) {};
		/*! Method to delete module: gets type of module, check availability of the module
		*
		*Throw exception if there is no module with this type
		*/
		void deleteModule(int type) {};
		/*! Method to turn module on: gets type of module, check availability of the module
		*
		* Throw exception if there is no module with this type
		*
		* Throw exception if module is already active
		*
		* It calls moduleOff in module
		*/
		void moduleOn(int type) {};
		/*!  Method to turn module off: gets type of module, check availability of the module
		*
		* Throw exception if there is no module with this type
		*
		* Throw exception if module is already inactive
		*
		* It calls moduleOff in module
		*/
		void moduleOff(int type) {};
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
		Component* managedComponents;  //!< array with managed devices
	public:
		//! Simple Constructor gets all variables for "Component" constructor and numD for numOfDevices
		managementComponent(int x0, int y0, int en, int num, int c, int numD, string desc = "Command center") :
			Component(x0, y0, desc, en, num, c), 
			numOfDevices(numD) {};
		//! Constructor to set num of managed devices
		managementComponent(int numD = 0) : numOfDevices(numD) {};
		//! virtual destructor to delete mananaged Components
		virtual ~managementComponent() { delete[] managedComponents; };
		/*! get environment information from robot
		*
		* throw exception, if there is no managed robot 
		*/
		EnvironmentInfo getInfo() {};
		/*! move robot
		* 
		* throw exception, if there is no managed robot 
		* 
		* it calls moveRobotInDirection function from mobileComponent
		*/
		virtual void moveRobot() {};
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
		//! Simple Constructor gets all variables for "Component" constructor and vel for velocity
		mobileComponent(int x0, int y0, string desc, int en = 0, int num = 0, int c = 0, int vel = 0) : 
			Component(x0, y0, desc, en, num, c), 
			velocity(vel) {};
		//! Constructor to set velocity of managed devices
		mobileComponent(int vel = 0) : velocity(vel) {};
		//! virtual destructor
		virtual ~mobileComponent() {};
		//! move robot in given direction: throw exception, if there is no managed robot 
		virtual void moveRobotInDirection(int direction) {};
	};

	//! Robot scout - mobile component
	/*! This class is robot scout
	*
	* Inheritor of "mobileComponent"
	*
	* Robot scout can be moved and it observe environment
	*/
	class robotScout : public mobileComponent {
	private:
	public:
		//! Simple Constructor gets all variables for "Component" and vel for "mobileComponent"
		robotScout(int x0, int y0, string desc = "Robot Scout", int en = 0, int num = 0, int c = 0, int vel = 0) : 
			Component(x0, y0, desc, en, num, c), 
			mobileComponent(vel) {};
		// destructor
		~robotScout() {};
	};

	//! Robot commander - mobile and management component
	/*! This class is robot Commander
	*
	* Public inheritor of virtual "mobileComponent" and virtual "managementComponent"
	*
	* Robot commander can be moved and it observe environment, also it can manage components
	*/
	class robotCommander : public mobileComponent, public managementComponent 
	{
	private:
	public:
		//! Simple Constructor gets all variables for "Component", vel for "mobileComponent", numD for "managementComponent"
		robotCommander(int x0, int y0, string desc = "Robot Commander", int en = 0, int num = 0, int c = 0, int numD = 0, int vel = 0) :
			Component(x0, y0, desc, en, num, c), 
			mobileComponent(vel), 
			managementComponent(numD) {};
		// destructor
	    ~robotCommander() {};
	};

}
#endif