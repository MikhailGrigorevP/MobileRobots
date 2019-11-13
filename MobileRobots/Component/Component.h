#include <iostream>
#include <string>
#include "../Module/Module.h"
#include "../Vector/vector.h"
using namespace my_std;
//using std::vector;
#pragma once;
using std::string;
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
		void setEnergy(int en) { energy = en; }

	public:

		//! constructors

		Component() :
			x(0),
			y(0),
			description("Observe center"),
			energy(0),
			slotsNum(0),
			cost(0) {

		};
		Component(int x0, int y0, string desc, int en, int num, int c);
		Component(Point point, string desc, int en, int num, int c);

		//! Destructor

		virtual ~Component();

		//! getters

		virtual int iAm() const { return observe_center; }

		Point getCoord() { return { x, y }; }
		string getDesc() { return description; }
		int getEnergy() { return energy; }
		int getSlotsNum() { return slotsNum; }
		int getModulesSize() { return modules.size(); }
		int getNum() { return slotsNum; }
		int getCost() { return cost; }
		virtual int getNumD() { return 0; }
		virtual int getVel() { return 0; }

		//! Modules getters
		vector<Module*>* getModules() {
			return &modules;
		}
		Module* getModule(int i) {
			if (i < 0)
				throw std::exception(" >>> incorrect num");
			if (i >= modules.size())
				throw std::exception(" >>> incorrect num");
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

		//! operators overload
		friend bool operator == (const Component& b, const Component& a)
		{
			return ((a.x == b.x) && (a.y == b.y));
		}
		friend bool operator != (const Component &b, const Component& a)
		{
			return ((a.x != b.x) || (a.y != b.y));
		}

		//! setters

		void setModule_g(int pr, int en, int c, int enpr);
		void setModule_s(int pr, int en, int c, int r, int ang, int direct);
		void setModule_m(int pr, int en, int c, int r, int n);

		//! Modules

		void moduleOn(int type);
		void moduleOff(int type);
		void deleteModule(int type);
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


		bool operator == (const managementComponent& a) const
		{
			return (a == *this);
		}

		vector<Component*>* getNComp() {
			return &managedComponents;
		} 

		Component* getComp(int i) {
			if (i < 0)
				throw std::exception(" >>> incorrect num");
			if (i >= managedComponents.size())
				throw std::exception(" >>> incorrect num");
			return managedComponents[i];
		}

		virtual ~managementComponent();

		virtual int getNumD() { return numOfDevices; }
		virtual int iAm() const { return command_center; }

		EnvironmentInfo getInfo(int i, ED_N::environmentDescriptor* env, vector<vector<unsigned>>& field = nofield);

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


		bool operator == (const mobileComponent& a) const
		{
			return (a == *this);
		}
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
		bool operator == (const robotScout& a) const
		{
			return (a == *this);
		}

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

		bool operator == (const robotCommander& a) const
		{
			return (a == *this);
		}

		robotCommander(int x0, int y0, string desc, int en, int num, int c, int numD = 0, int vel = 0);
		virtual int iAm() const { return robot_commander; }
	    ~robotCommander();
	};

}
#endif