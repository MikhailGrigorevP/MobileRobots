#pragma once

//! Point of the field
/*! Using for environment
*/
struct Point {
	int x = 0;   //!< x coordinate
	int y = 0;   //!< y coordinate
};

//! enum of modules' type for methods
enum ModuleType { managemen_tModule, generator_Module, sensor_Module };
//! enum of directions' type for movement
enum Directions { left, right, up, down };

//! struct for environment information
struct EnvironmentInfo {

};