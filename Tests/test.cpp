#include "pch.h"

#include <initializer_list>
#include <stdexcept>
#include <memory>

#include "../MobileRobots/Vector/vector.h"
#include "../MobileRobots/environmentDescriptor/environmentDescriptor.cpp"
#include "../MobileRobots/environmentDescriptor/environmentDescriptor.h"
#include "../MobileRobots/Component/Component.h"
#include "../MobileRobots/Component/Component.cpp"
#include "../MobileRobots/Module/Module.h"
#include "../MobileRobots/Module/Module.cpp"
#include "../MobileRobots/Structures/structures.h"



TEST(environmentDescriptorT, ED_constructor) {

	ED_N::environmentDescriptor environment;

	Field_size point = { 0, 0 };
	EXPECT_EQ(point, environment.getSize());

}

TEST(environmentDescriptorT, ED_setters) {

	ED_N::environmentDescriptor environment(16, 16);

	environment.setSize({ 10, 10 });
	Field_size point = { 10, 10 };
	EXPECT_EQ(point, environment.getSize());

	environment.setSize({ 20, 20 });
	point = { 20, 20 };
	EXPECT_EQ(point, environment.getSize());

	EXPECT_EQ(none_cell, environment.getCell({ 0, 6 }));

	environment.setCell({ 8, 11 }, interest_point);
	EXPECT_EQ(interest_point, environment.getCell({ 8, 11 }));

}

TEST(environmentDescriptorT, ED_exception) {

	ED_N::environmentDescriptor environment;

	ASSERT_ANY_THROW(environment.setSize({ -2, 10 }));
	ASSERT_ANY_THROW(environment.setSize({ 5, -5 }));

	ASSERT_ANY_THROW(environment.getCell({ 0, 6 }));
	ASSERT_ANY_THROW(environment.setCell({ 8, 11 }, interest_point));

	environment.setSize({ 20, 20 }); //- גûחûגאוע סבמי

	ASSERT_ANY_THROW(environment.getCell({ -6, 6 }));
	ASSERT_ANY_THROW(environment.getCell({ 6, -6 }));
	ASSERT_ANY_THROW(environment.getCell({ 66, 6 }));
	ASSERT_ANY_THROW(environment.getCell({ 6, 66 }));
	ASSERT_ANY_THROW(environment.setCell({ 8, -11 }, interest_point));
	ASSERT_ANY_THROW(environment.setCell({ -8, 11 }, interest_point));
	ASSERT_ANY_THROW(environment.setCell({ 8, 111 }, interest_point));
	ASSERT_ANY_THROW(environment.setCell({ 88, 11 }, interest_point));

}

TEST(ComponentT, C_constructors) {

	Components_N::Component component_0;

	Point point = { 0, 0 };
	EXPECT_EQ(point, component_0.getCoord());
	EXPECT_EQ(0, component_0.getCost());
	EXPECT_EQ("Observe center", component_0.getDesc());
	EXPECT_EQ(0, component_0.getEnergy());
	EXPECT_EQ(0, component_0.getSlotsNum());

	Components_N::Component component_1(0, 6, "Constructor", 100, 4, 250);

	point = { 0, 6 };
	EXPECT_EQ(point, component_1.getCoord());
	EXPECT_EQ(250, component_1.getCost());
	EXPECT_EQ("Constructor", component_1.getDesc());
	EXPECT_EQ(100, component_1.getEnergy());
	EXPECT_EQ(4, component_1.getSlotsNum());

	Components_N::robotScout component_2(0, 6, "Constructor", 100, 4, 250, 2);

	point = { 0, 6 };
	EXPECT_EQ(point, component_2.getCoord());
	EXPECT_EQ(250, component_2.getCost());
	EXPECT_EQ("Constructor", component_2.getDesc());
	EXPECT_EQ(100, component_2.getEnergy());
	EXPECT_EQ(4, component_2.getSlotsNum());
	EXPECT_EQ(2, component_2.getVel());

	Components_N::robotCommander component_3(0, 6, "Constructor", 100, 4, 250, 5, 2);

	point = { 0, 6 };
	EXPECT_EQ(point, component_3.getCoord());
	EXPECT_EQ(250, component_3.getCost());
	EXPECT_EQ("Constructor", component_3.getDesc());
	EXPECT_EQ(100, component_3.getEnergy());
	EXPECT_EQ(4, component_3.getSlotsNum());
	EXPECT_EQ(5, component_3.getNumD());
	EXPECT_EQ(2, component_3.getVel());

	Components_N::managementComponent component_4(0, 6, 100, 4, 250, 5, "Constructor");

	point = { 0, 6 };
	EXPECT_EQ(point, component_4.getCoord());
	EXPECT_EQ(250, component_4.getCost());
	EXPECT_EQ("Constructor", component_4.getDesc());
	EXPECT_EQ(100, component_4.getEnergy());
	EXPECT_EQ(4, component_4.getSlotsNum());
	EXPECT_EQ(5, component_4.getNumD());

}

TEST(ComponentT, C_move) {

	Components_N::robotScout component(0, 6, "Constructor", 100, 4, 250, 2);

	Point point = { 0, 6 };
	EXPECT_EQ(point, component.getCoord());

	component.moveRobotInDirection(right);

	point = { 2, 6 };
	EXPECT_EQ(point, component.getCoord());

	component.moveRobotInDirection(down);

	point = { 2, 8 };
	EXPECT_EQ(point, component.getCoord());

	component.moveRobotInDirection(up);

	point = { 2, 6 };
	EXPECT_EQ(point, component.getCoord());

	component.moveRobotInDirection(left);

	point = { 0, 6 };
	EXPECT_EQ(point, component.getCoord());

}

TEST(ComponentT, C_modules) {

	Components_N::managementComponent component(0, 6, 100, 4, 250, 5, "Constructor");

	component.setModule_g(1, 150, 180, 5000);
	component.setModule_s(1, 150, 180, 8, 180, left);

	EXPECT_EQ(0, component.getSModule(1)->getState());
	component.moduleOn(1);
	EXPECT_EQ(1, component.getSModule(1)->getState());
	component.moduleOff(1);
	EXPECT_EQ(0, component.getSModule(1)->getState());

	component.deleteModule(1);
	ASSERT_ANY_THROW(component.moduleOn(1));
}


TEST(ComponentT, C_modules_Exception) {

	Components_N::managementComponent component(0, 6, 100, 4, 250, 5, "Constructor");

	component.setModule_g(1, 150, 180, 5000);
	component.moduleOff(0);
	component.setModule_s(1, 150, 180, 8, 180, left);

	ASSERT_ANY_THROW(component.moduleOn(1));

	component.moduleOn(0);
	component.moduleOn(1);

	component.setModule_s(1, 9000, 180, 8, 180, left);
	ASSERT_ANY_THROW(component.moduleOn(2));

}

TEST(ComponentT, C_management) {

	Components_N::managementComponent component(0, 3, 100, 4, 250, 5, "Constructor");
	component.setModule_g(1, 150, 180, 5000);
	component.setModule_m(1, 150, 180, 8, 5);
	component.moduleOn(1);

	Components_N::robotScout component_1(0, 6, "Constructor", 100, 4, 250, 2);
	Point point = { 0, 6 };
	EXPECT_EQ(point, component_1.getCoord());

	Components_N::robotScout component_2(0, 9, "Constructor", 100, 4, 250, 2);

	component.getMModule(1)->sendResourse(&component, &component_1);
	component.moveRobot(0, right);
	point = { 2, 6 };
	EXPECT_EQ(point, component_1.getCoord());

	component.getMModule(1)->sendResourse(&component, &component_2);
	component.getMModule(1)->freeResourse(&component, 0);

	point = { 0, 9 };
	EXPECT_EQ(point, component.getComp(0)->getCoord());
	ASSERT_ANY_THROW(component.getComp(1));

}

TEST(ComponentT, C_management_info) {

	ED_N::environmentDescriptor environment;

	environment.setSize({ 16, 16 });
	environment.setCell({ 0, 0 }, 2);
	environment.setCell({ 0, 5 }, 1);
	environment.setCell({ 0, 6 }, 1);
	environment.setCell({ 2, 1 }, 1);
	environment.setCell({ 9, 1 }, 2);
	environment.setCell({ 12, 1 }, 1);
	environment.setCell({ 7, 3 }, 1);
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

	//0
	environment.AddRobotScout({ 5, 2 }, 80, 4, 2, 600);
	environment.getComponent(0)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(0)->moduleOn(0);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 0);
	environment.getComponent(0)->moduleOn(1);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 1);
	environment.getComponent(0)->moduleOn(2);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 2);
	environment.getComponent(0)->moduleOn(3);

	//1
	environment.AddRobotScout({ 1, 6 }, 80, 5, 2, 600);
	environment.getComponent(1)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(1)->moduleOn(0);
	environment.getRSComponent(1)->setModule_s(1, 150, 500, 4, 60, 0);
	environment.getComponent(1)->moduleOn(1);
	environment.getRSComponent(1)->setModule_s(1, 150, 500, 4, 60, 1);
	environment.getComponent(1)->moduleOn(2);
	environment.getRSComponent(1)->setModule_s(1, 150, 500, 4, 60, 2);
	environment.getComponent(1)->moduleOn(3);

	//2
	environment.AddRobotCommander({ 1, 2 }, 150, 4, 2, 1, 600);
	environment.getComponent(2)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(2)->moduleOn(0);
	environment.getComponent(2)->setModule_m(2, 150, 250, 9, 4);
	environment.getRCComponent(2)->getMModule(1)->sendResourse(environment.getRCComponent(2), environment.getComponent(0));
	environment.getRCComponent(2)->getMModule(1)->sendResourse(environment.getRCComponent(2), environment.getComponent(1));

	EnvironmentInfo envInf, envInfo2;

	envInf = environment.getRCComponent(2)->getInfo(0, &environment);
	envInfo2 = environment.getRCComponent(2)->getInfo(1, &environment);

	vector<Point> points = { { 2, 1 }, { 2, 4 }, { 7, 3 } };

	EXPECT_EQ(points, envInf.barriers);

	points.clear();
	points.push_back({ 9, 1 });
	EXPECT_EQ(points, envInf.pointsOfInterest);
	points.push_back({ 5, 1 });
	//EXPECT_NE(points2, envInf.pointsOfInterest);

	EXPECT_EQ(environment.getRCComponent(2), envInfo2.components[0]);
	EXPECT_NE(environment.getRCComponent(1), envInfo2.components[0]);


}


TEST(ModuleT, M_info) {

	ED_N::environmentDescriptor environment;

	environment.setSize({ 16, 16 });
	environment.setCell({ 0, 0 }, 2);
	environment.setCell({ 0, 5 }, 1);
	environment.setCell({ 0, 6 }, 1);
	environment.setCell({ 2, 1 }, 1);
	environment.setCell({ 9, 1 }, 2);
	environment.setCell({ 12, 1 }, 1);
	environment.setCell({ 8, 3 }, 1);
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

	//0
	environment.AddRobotScout({ 5, 2 }, 80, 4, 2, 600);
	environment.getComponent(0)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(0)->moduleOn(0);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 0);
	environment.getComponent(0)->moduleOn(1);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 1);
	environment.getComponent(0)->moduleOn(2);
	environment.getRSComponent(0)->setModule_s(1, 150, 500, 4, 60, 2);
	environment.getComponent(0)->moduleOn(3);

	//1
	environment.AddRobotScout({ 1, 6 }, 80, 5, 2, 600);
	environment.getComponent(1)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(1)->moduleOn(0);
	environment.getRSComponent(1)->setModule_s(1, 150, 500, 4, 60, 0);
	environment.getComponent(1)->moduleOn(1);
	environment.getRSComponent(1)->setModule_s(1, 150, 500, 4, 60, 1);
	environment.getComponent(1)->moduleOn(2);
	environment.getRSComponent(1)->setModule_s(1, 150, 500, 4, 60, 2);
	environment.getComponent(1)->moduleOn(3);

	//2
	environment.AddRobotCommander({ 1, 2 }, 150, 4, 2, 1, 600);
	environment.getComponent(2)->setModule_g(1, 100, 125, 4500);
	environment.getComponent(2)->moduleOn(0);
	environment.getComponent(2)->setModule_m(2, 150, 250, 9, 4);


	EnvironmentInfo envInf, envInfo2;

	Point point = environment.getRSComponent(0)->getCoord();
	envInf = environment.getRSComponent(0)->getSModule(2)->getInfo(point, &environment);

	point = environment.getRSComponent(1)->getCoord();
	envInfo2 = environment.getRSComponent(1)->getSModule(3)->getInfo(point, &environment);

	vector<Point> points;
	points.push_back({ 8, 3 });

	EXPECT_EQ(points, envInf.barriers);

	points.clear();
	points.push_back({ 9, 1 });
	EXPECT_EQ(points, envInf.pointsOfInterest);
	points.push_back({ 5, 1 });
	//EXPECT_NE(points2, envInf.pointsOfInterest);

	EXPECT_EQ(environment.getRCComponent(2), envInfo2.components[0]);
	EXPECT_NE(environment.getRCComponent(1), envInfo2.components[0]);

}

TEST(ModuleT, M_constructors) {

	Modules_N::generatorModule modul(1, 1, 150, 180, 5000);

	EXPECT_EQ(180, modul.getCost());
	EXPECT_EQ(150, modul.getEnergy());
	EXPECT_EQ(4850, modul.getEnergyProvision());
	EXPECT_EQ(1, modul.getPriority());
	EXPECT_EQ(1, modul.getState());

	Modules_N::managementModule modul_1(1, 1, 150, 180, 8, 4);

	EXPECT_EQ(180, modul_1.getCost());
	EXPECT_EQ(150, modul_1.getEnergy());
	EXPECT_EQ(4, modul_1.getN());
	EXPECT_EQ(8, modul_1.getR());
	EXPECT_EQ(1, modul_1.getPriority());
	EXPECT_EQ(1, modul_1.getState());

	Modules_N::sensorModule modul_2(1, 1, 150, 180, 8, 180, left);

	EXPECT_EQ(180, modul_2.getCost());
	EXPECT_EQ(150, modul_2.getEnergy());
	EXPECT_EQ(1, modul_2.getPriority());
	EXPECT_EQ(1, modul_2.getState());
	EXPECT_EQ(180, modul_2.getAng());
	EXPECT_EQ(left, modul_2.getDir());
	EXPECT_EQ(8, modul_2.getR());

}

TEST(ModuleT, M_toggle) {

	Modules_N::generatorModule modul(1, 1, 150, 180, 5000);

	modul.off();
	EXPECT_EQ(0, modul.getState());

	modul.on();
	EXPECT_EQ(1, modul.getState());

}