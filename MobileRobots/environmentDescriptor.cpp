#include "environmentDescriptor.h"

namespace ED_N {


	//! Constructor
	environmentDescriptor::environmentDescriptor(int m0, int n0) : size{ m0, n0} {};

	//! Constructor
	environmentDescriptor::environmentDescriptor(Field_size field_size) : size{ field_size.m, field_size.n } {};

	//! Destructor
	environmentDescriptor::~environmentDescriptor() {
		field.~vector();
		components.~vector();
	};

	//! get size of field
	Field_size environmentDescriptor::getSize() {
		return size;
	};

	//! set size of field
	void environmentDescriptor::setSize(Field_size field_size) {
		size.m = field_size.m;
		size.n = field_size.n;

		for (int i = 0; i < size.n; ++i) {
			field.push_back(std::vector<unsigned>());
			for (int j = 0; j < size.m; ++j) {
				field[i].push_back(none_cell);
			}
		}

	};

	//! get type of cell
	unsigned environmentDescriptor::getCell(Point point) {
		if (size.m == 0 || size.n == 0)
			throw std::exception(">>> Environment is empty");
		if (point.x >= size.n)
			throw std::exception(" >>> x coordinate bigger then horizontal size");
		if (point.y >= size.m)
			throw std::exception(" >>> y coordinate bigger then vertical size");
		return field[point.x].at(point.y);
	};

	//! set type of cell
	void environmentDescriptor::setCell(Point point, unsigned cell_type) {
		if (size.m == 0 || size.n == 0)
			throw std::exception(">>> Environment is empty");
		if(point.x >= size.n)
			throw std::exception(" >>> x coordinate bigger then horizontal size");
		if (point.y >= size.m)
			throw std::exception(" >>> y coordinate bigger then vertical size");
		if ((cell_type != barrier) && (cell_type != interest_point))
			throw std::exception(" >>> incorrect type of cell");

		field[point.x].at(point.y) = cell_type;
	};

	// Input
	std::istream& operator >> (std::istream& s, environmentDescriptor& envD) {

		std::cout << ">> ";
		Field_size field_size;
		s >> field_size.m >> field_size.n;
		if (std::cin.fail())
			throw std::exception(" >>> input failed");
		envD.setSize(field_size);

		return s;
	}

	// Output
	std::ostream& operator << (std::ostream& o , const environmentDescriptor& envD) {
		if (envD.size.m == 0 || envD.size.n == 0) 
			throw std::exception(">>> Environment is empty");

		else
			o << ">>> Environment info: " << envD.size.m << " x " << envD.size.n << "\n";
		return o;
	}


	void environmentDescriptor::AddCommandCenter(Point point, int en, int num, int numD, int c) {

		if (size.m == 0 || size.n == 0)
			throw std::exception(">>> Environment is empty");
		if (point.x >= size.n)
			throw std::exception(" >>> x coordinate bigger then horizontal size");
		if (point.y >= size.m)
			throw std::exception(" >>> y coordinate bigger then vertical size");
		if (en <= 0)
			throw std::exception(" >>> incorrect energy level");
		if (num <= 0)
			throw std::exception(" >>> incorrect slots num");
		if (c <= 0)
			throw std::exception(" >>> incorrect cost");
		if (numD <= 0)
			throw std::exception(" >>> incorrect num of managed modules");

		managementComponent* component = new managementComponent (point.x, point.y, en, num, c, numD, "Command center");
		components.push_back(component);
	};

	void environmentDescriptor::AddRobotCommander(Point point, int en, int num, int numD, int vel, int c) {

		if (size.m == 0 || size.n == 0)
			throw std::exception(">>> Environment is empty");
		if (point.x >= size.n)
			throw std::exception(" >>> x coordinate bigger then horizontal size");
		if (point.y >= size.m)
			throw std::exception(" >>> y coordinate bigger then vertical size");
		if (en <= 0)
			throw std::exception(" >>> incorrect energy level");
		if (num <= 0)
			throw std::exception(" >>> incorrect slots num");
		if (numD <= 0)
			throw std::exception(" >>> incorrect num of managed modules");
		if (c <= 0)
			throw std::exception(" >>> incorrect cost");
		if (vel <= 0)
			throw std::exception(" >>> incorrect velocity");


		robotCommander* component = new robotCommander(point.x, point.y, "Robot commander", en, num, c, numD, vel);
			components.push_back(component);
	};

	void environmentDescriptor::AddRobotScout(Point point, int en, int num, int vel, int c) {

		if (size.m == 0 || size.n == 0)
			throw std::exception(">>> Environment is empty");
		if (point.x >= size.n)
			throw std::exception(" >>> x coordinate bigger then horizontal size");
		if (point.y >= size.m)
			throw std::exception(" >>> y coordinate bigger then vertical size");
		if (en <= 0)
			throw std::exception(" >>> incorrect energy level");
		if (num <= 0)
			throw std::exception(" >>> incorrect slots num");
		if (c <= 0)
			throw std::exception(" >>> incorrect cost");
		if (vel <= 0)
			throw std::exception(" >>> incorrect velocity");

		robotScout* component = new robotScout(point.x, point.y, "Robot commander", en, num, c, vel);
		components.push_back(component);

	};

	void environmentDescriptor::AddObserveCenter(Point point, int en, int num, int c) {

		if (size.m == 0 || size.n == 0)
			throw std::exception(" >>> Environment is empty");
		if (point.x >= size.n)
			throw std::exception(" >>> x coordinate bigger then horizontal size");
		if (point.y >= size.m)
			throw std::exception(" >>> y coordinate bigger then vertical size");
		if (en <= 0)
			throw std::exception(" >>> incorrect energy level");
		if (num <= 0)
			throw std::exception(" >>> incorrect slots num");
		if (c <= 0)
			throw std::exception(" >>> incorrect cost");

		Component* component = new Component (point.x, point.y, "Observe center", en, num, c);
		components.push_back(component);


	}

	void environmentDescriptor::showComponents() {
		if (size.m == 0 || size.n == 0)
			throw std::exception(">>> Environment is empty");


		if (components.size() == 0)
			std::cout << "No any components in field: \n";

		else {
			std::cout << "Componets in field: \n";
			for (int i = 0; i < components.size(); ++i) {
				switch (components[i]->iAm()) {
				case observe_center:
					std::cout << i << ")  Observe center (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ")" << std::endl;
					break;
				case command_center:
					std::cout << i << ")  Command center: (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ")" << std::endl;
					break;
				case robot_commander:
					std::cout << i << ")  Robot commander: (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ")" << std::endl;
					break;
				case robot_scout:
					std::cout << i << ")  Robot scout: (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ")" << std::endl;
					break;
				}
				if (components[i]->getModulesSize() == 0)
					std::cout << "\tNo modules in this components";
				else
					for (int j = 0; j < components[i]->getModulesSize(); ++j) {
						std::cout << "\t" << j << ". ";
						switch ((components[i]->getModule(j))->iAm()) {
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
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
	}

	void environmentDescriptor::drawMap() {
		if (size.m == 0 || size.n == 0)
			throw std::exception(">>> Environment is empty");

		for (int j = 0; j < size.m; ++j) {
			for (int i = 0; i < size.n; ++i) {
				switch (field[i].at(j)) {
				case none_cell:
					std::cout << "~";
					break;
				case barrier:
					std::cout << "#";
					break;
				case interest_point:
					std::cout << "*";
					break;
				}
			}
			std::cout << std::endl;
		}
		std::cout << "Where:\n" << "\t~ - is free cell\n" << "\t# - is barrier\n" << "\t* - is point of interest\n\n";

		if(components.size() == 0)
			std::cout << "No any components in field: \n";

		else {

			std::cout << "Componets in field: \n";

			for (int i = 0; i < components.size(); ++i) {
				switch (components[i]->iAm()) {
				case observe_center:
					std::cout << i + 1 << ")  Observe center with (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ") coordinates, it needs: " << components[i]->getEnergy() <<
						" energy to work, it's support " << components[i]->getNum() << " modules. Cost: " << components[i]->getCost() << std::endl;
					std::cout << "\tDesription: " << components[i]->getDesc() << std::endl;
					break;
				case command_center:
					std::cout << i + 1 << ")  Command center with (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ") coordinates, it needs: " << components[i]->getEnergy() <<
						" energy to work, it's support " << components[i]->getNum() << " modules and can control " << components[i]->getNumD() << " components. Cost: " << components[i]->getCost() << std::endl;
					std::cout << "\tDesription: " << components[i]->getDesc() << std::endl;
					break;
				case robot_commander:
					std::cout << i + 1 << ")  Robot commander with (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ") coordinates, it needs: " << components[i]->getEnergy() <<
						" energy to work, it's support " << components[i]->getNum() << " modules and can control " << components[i]->getNumD() << " components. It's velocity: " << components[i]->getVel() << ". Cost: " << components[i]->getCost() << std::endl;
					std::cout << "\tDesription: " << components[i]->getDesc() << std::endl;
					break;
				case robot_scout:
					std::cout << i + 1 << ")  Robot scout with (" << components[i]->getCoord().x << ";" << components[i]->getCoord().y << ") coordinates, it needs: " << components[i]->getEnergy() <<
						" energy to work, it's support " << components[i]->getNum() << " modules and it's velocity: " << components[i]->getVel() << ". Cost: " << components[i]->getCost() << std::endl;
					std::cout << "\tDesription: " << components[i]->getDesc() << std::endl;
					break;
				}

				if (components[i]->getModulesSize() == 0)
					std::cout << "\tNo modules in this components";
				else
					for (int j = 0; j < components[i]->getModulesSize(); ++j) {
						std::cout << "\t" << j << ". ";
						switch ((components[i]->getModule(j))->iAm()) {
						case generator_Module:
							std::cout << "Generator Module with prioritet - " << components[i]->getGModule(j)->getPriority() << 
								", energy - " << components[i]->getGModule(j)->getEnergy() << 
								", cost - " << components[i]->getGModule(j)->getCost() << 
								", state - " << components[i]->getGModule(j)->getState() <<
								", energy provision - " << components[i]->getGModule(j)->getEnergyProvision();
							break;
						case sensor_Module:
							std::cout << "Sensor Module with prioritet - " << components[i]->getSModule(j)->getPriority() <<
								", energy - " << components[i]->getSModule(j)->getEnergy() <<
								", cost - " << components[i]->getSModule(j)->getCost() <<
								", state - " << components[i]->getSModule(j)->getState() <<
								", radius - " << components[i]->getSModule(j)->getR() <<
								", angle - " << components[i]->getSModule(j)->getAng() <<
								", direction - " << components[i]->getSModule(j)->getDir();
							break;
						case management_Module:
							std::cout << "Management Module with prioritet - " << components[i]->getMModule(j)->getPriority() <<
								", energy - " << components[i]->getMModule(j)->getEnergy() <<
								", cost - " << components[i]->getMModule(j)->getCost() <<
								", state - " << components[i]->getMModule(j)->getState() <<
								", radius - " << components[i]->getMModule(j)->getR() <<
								", angle - " << components[i]->getMModule(j)->getN();
							break;

						}
						std::cout << std::endl;
					}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

	}

}