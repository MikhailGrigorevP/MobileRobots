#include <iostream>
#include "../Structures/structures.h"

#ifndef _MODULE_H_
#define _MODULE_H_

namespace ED_N {
	class environmentDescriptor;
}
namespace Components_N {
	class Component;
	class managementComponent;
}
//!Modules namespace
/*! All modules for components:
 * 1. generatorModule
 *
 * 2. managementModule
 *
 * 3. sensorModule
 *
 */
namespace Modules_N {

	//! Parent class
	class Module
	{
	private:
		int priority = 0;  //!< Priority of module
		int energy = 0;  //!< Energy of module
		int cost = 0;  //!< Cost of module
	protected:
		bool state = 0;  //!< State of module
	public:
		/*! Simple Module gets
		* st to set state,
		* pr to set priority,
		* en to set energy,
		* c to set cost
		*/
		//! turn module on
		virtual void on();
		//! turn module off
		virtual void off();
		Module(int st = 0, int pr = 0, int en = 0, int c = 0) : 
			state(st), 
			priority(pr), 
			energy(en), 
			cost(c) {};
		//! virtual destructor
		virtual  ~Module() {};
		virtual int iAm() const = 0;
		//! get energy of module 

		virtual int getEnergy() {
			return energy;
		};
		virtual int getCost() {
			return cost;
		};
		virtual int getPriority() {
			return priority;
		};
		virtual bool getState() {
			return state;
		};
	};

	//! Generator
	/*! Generator
	*
	* inheritor of Module with energy provision
	*
	*/
	class generatorModule : public Module
	{
	private:
		int energyProvision = 0;  //!< energy than generator produce
	protected:
	public:
		virtual int iAm() const { return generator_Module; }
		//!	Simple constructor
		generatorModule(int st = 0, int pr = 0, int en = 0, int c = 0, int enpr = 0) : 
			Module(st, pr, en, c), 
			energyProvision(enpr) {};

		virtual int getEnergyProvision() {
			return energyProvision - Module::getEnergy();
		};
		//! Destructor
		~generatorModule() {};
	};

	//! management Module
	/*! management Module
	*
	* inheritor of Module with radius and num of managed components
	*
	*/
	class managementModule : public Module
	{
	private:
		int radius = 0;  //!< range of the module
		int num = 0;  //!< num of managed components
	protected:


	public:
		//!< send management resourse
		void sendResourse(Components_N::managementComponent*, Components_N::Component*);

		//!< free management resourse
		void freeResourse(Components_N::Component*, int i);
		virtual int getR() {
			return radius;
		};
		virtual int getN() {
			return num;
		};
		virtual int iAm() const { return management_Module; }
		//!	Simple constructor
		managementModule(int st = 0, int pr = 0, int en = 0, int c = 0, int r = 0, int n = 0) :
			Module(st, pr, en, c), 
			radius(r), 
			num(n) {};
		//! Destructor
		~managementModule() {};
	};

	//! Sensor
	/*! Sensor
	*
	* inheritor of Module with radiusá angle and direction
	*
	*/
	class sensorModule : public Module
	{
	private:
		int radius = 0;
		int angle = 0;
		int direction = 0;
	protected:
	public:

		EnvironmentInfo getInfo(Point curr_location, ED_N::environmentDescriptor* env);

		virtual int getR() {
			return radius;
		};
		virtual int getAng() {
			return angle;
		};
		virtual int getDir() {
			return direction;
		};
		virtual int iAm() const { return sensor_Module; }
		//!	Simple constructor
		sensorModule(int st = 0, int pr = 0, int en = 0, int c = 0, int r = 0, int ang = 0, int direct = 0) :
			Module(st, pr, en, c), 
			radius(r), 
			angle(ang), 
			direction(direct) {};
		//! Destructor
		~sensorModule() {};

	};


}
#endif