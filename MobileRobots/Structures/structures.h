#pragma once

#include <vector>
using std::vector;

//#include "../Vector/vector.h"
//using namespace my_std;
#include <string>
#include <iostream>

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

	friend bool operator == (const Point& b, const Point& a)
	{
		return ((a.x == b.x) && (a.y == b.y));
	}

	friend bool operator != (const Point& b, const Point& a)
	{
		return ((a.x != b.x) || (a.y != b.y));
	}

	friend bool operator < (const Point& b, const Point& a)
	{
		if (b.x < a.x)
			return true;
		else if (b.x > a.x)
			return false;
		else if (b.y < a.y)
			return true;

		return false;
	}
};

enum VisitorStatus { V_not, V_partly, V_visited };

struct Vertex {
	int x = 0;   //!< x coordinate
	int y = 0;   //!< y coordinate
	int visited = V_not;

	friend bool operator == (const Vertex& b, const Vertex& a)
	{
		return ((a.x == b.x) && (a.y == b.y));
	}

	friend bool operator != (const Vertex& b, const Vertex& a)
	{
		return ((a.x != b.x) || (a.y != b.y));
	}

	friend bool operator < (const Vertex& b, const Vertex& a)
	{
		if (b.x < a.x)
			return true;
		else if (b.x > a.x)
			return false;
		else if (b.y < a.y)
			return true;

		return false;
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
enum CellType { none_cell, barrier, interest_point, notexist, ai_seen, rc, rs, rsd, cc, oc, border };
//! enum of components' type for methods
enum ComponentType { observe_center, command_center, robot_commander, robot_scout };
//! enum of modules' type for methods
enum ModuleType { management_Module, generator_Module, sensor_Module };

//! enum of directions' type for movement
enum DirectionType { left, right, up, down };

enum Commander_mode { waiting_mode, following_mode, state_mode };

//! struct for environment information
struct EnvironmentInfo {
	vector<Components_N::Component*> components;  //!< used components
	vector<Point> pointsOfInterest;  //!< points of interest
	vector<Point> barriers;  //!< barriers
};