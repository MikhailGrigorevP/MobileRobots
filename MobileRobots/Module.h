#include <iostream>
#include "structures.h"

#ifndef _MODULE_H_
#define _MODULE_H_

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
		bool state = 0;  //!< State of module
		int priority = 0;  //!< Priority of module
		int energy = 0;  //!< Energy of module
		int cost = 0;  //!< Cost of module
	protected:
		//! turn module on
		virtual void on() {};
		//! turn module off
		virtual void off() {};
	public:
		/*! Simple Module gets
		* st to set state,
		* pr to set priority,
		* en to set energy,
		* c to set cost
		*/
		Module(int st = 0, int pr = 0, int en = 0, int c = 0) : 
			state(st), 
			priority(pr), 
			energy(en), 
			cost(c) {};
		//! virtual destructor
		virtual  ~Module() {};
		//! get energy of module 
		virtual int getEnergy() {};
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
		//!	Simple constructor
		generatorModule(int st = 0, int pr = 0, int en = 0, int c = 0, int enpr = 0) : 
			Module(st, pr, en, c), 
			energyProvision(enpr) {};
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
		//!< send management resourse
		void sendResourse() {};
		//!< free management resourse
		void freeResourse() {};
	public:
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
		//!get information from environment
		EnvironmentInfo getInfo() {};
	public:
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