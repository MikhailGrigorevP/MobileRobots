#pragma once

#include <vector>
#include <string>
#include <iostream>
using std::vector;

namespace ED_N {
	class environmentDescriptor;
}
namespace Components_N {
	class Component;
}
//! Point of the field
/*! Using for environment
*/
struct Point {
	int x = 0;   //!< x coordinate
	int y = 0;   //!< y coordinate

	bool operator == (const Point& a) const
	{
		return (x == a.x && y == a.y);
	}
};

//////! Cell of the field
/////*! Using for environment
////*/
////struct Cell {
////	Point point = { 0, 0 };   //!< x and y coordinate
////	unsigned cell = none_cell;   //!< cell type
////};

//! Size of the field
/*! Using for environment
*/
struct Field_size {
	int m = 0;   //!< max x size
	int n = 0;   //!< max y size

	bool operator == (const Field_size& a) const
	{
		return (m == a.m && n == a.n);
	}
};

//! enum of cell's type of field
enum CellType { none_cell, barrier, interest_point, notexist };
//! enum of components' type for methods
enum ComponentType { observe_center, command_center, robot_commander, robot_scout };
//! enum of modules' type for methods
enum ModuleType { management_Module, generator_Module, sensor_Module };
//! enum of directions' type for movement
enum DirectionType { left, right, up, down };

//! struct for environment information
struct EnvironmentInfo {
	vector<Components_N::Component*> components;  //!< used components
	vector<Point> pointsOfInterest;  //!< points of interest 
	vector<Point> barriers;  //!< barriers 

};