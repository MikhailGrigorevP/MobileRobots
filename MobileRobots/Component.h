#include <iostream>
#include <string>
#include "Module.h"

using std::string;

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

namespace Components_N {

	//This class is also "Observe center"
	class Component{
	protected:
		int x = 0, y = 0;
		string description;
		int energy = 0;
		const int slotsNum = 0;
		Modules_N::Module* modules;
		int cost = 0;
	public:
		Component(int x0 = 0, int y0 = 0):x(x0), y(y0) {};
		virtual ~Component() {};
		void setModule() {};
		void deleteModule() {};
		void moduleOn() {};
		void moduleOff() {};
	};

	//This class is also "Command center"
	class managementComponent : public virtual  Component 
	{
	protected:
		int numOfDevices = 0;
	public:
		managementComponent(int x0, int y0, int num) : Component(x0, y0), numOfDevices(num) {};
		managementComponent(int num = 0) : numOfDevices(num) {};
		virtual ~managementComponent() {};
		void getInfo() {};
		void moveRobot() {};
	};

	class mobileComponent : public virtual  Component 
	{
	protected:
		int velocity = 0;
	public:
		mobileComponent(int x0, int y0, int vel) : Component(x0, y0), velocity(vel) {};
		mobileComponent(int vel = 0) : velocity(vel) {};
		virtual ~mobileComponent() {};
		void move() {};
	};

	class robotScout : public mobileComponent {
	private:
	public:
		robotScout(int x0 = 0, int y0 = 0, int vel = 0) : Component(x0, y0), mobileComponent(vel) {};
		~robotScout() {};
	};

	class robotCommander : public mobileComponent, public managementComponent 
	{
	private:
	public:
		robotCommander(int x0 = 0, int y0 = 0, int vel = 0, int num = 0) : Component(x0, y0), mobileComponent(vel), managementComponent(num) {};
	    ~robotCommander() {};
	};

}
#endif