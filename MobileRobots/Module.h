#include <iostream>

#ifndef _MODULE_H_
#define _MODULE_H_

namespace Modules_N {
	class Module
	{
	protected:
		bool state = 0;
		int priority = 0;
		int energy = 0;
		int cost = 0;
	public:
		Module(int st = 0, int pr = 0, int en = 0, int c = 0) : state(st), priority(pr), energy(en), cost(c) {};
		~Module() {};
		virtual void on() {};
		virtual void off() {};
		virtual int getEnergy() {};
	};

	class generatorModule : public Module
	{
	private:
		int energyLevel = 0;
	public:
		generatorModule(int st = 0, int pr = 0, int en = 0, int c = 0, int enlvl = 0) : Module(st, pr, en, c), energyLevel(enlvl) {};
		~generatorModule() {};
	};

	class managementModule : public Module
	{
	private:
		int radius = 0;
	public:
		managementModule(int st = 0, int pr = 0, int en = 0, int c = 0, int r = 0) :Module(st, pr, en, c), radius(r) {};
		~managementModule() {};
		void sendResourse() {};
		void freeResourse() {};
	};

	class sensorModule : public Module
	{
	private:
		int radius = 0;
		int angle = 0;
		int direction = 0;
	public:
		sensorModule(int st = 0, int pr = 0, int en = 0, int c = 0, int r = 0, int ang = 0, int direct = 0) :Module(st, pr, en, c), radius(r), angle(angl), direction(direct) {};
		~sensorModule() {};
		void getInfo() {};

	};


}
#endif