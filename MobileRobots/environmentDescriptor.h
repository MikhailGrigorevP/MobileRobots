#ifndef _ENVIRONMENTDESCRIPTOR_H_
#define _ENVIRONMENTDESCRIPTOR_H_

#include "Component.h"
using std::vector;
//! Environment desriptor namespace
/*! ED namespace
*/
namespace ED_N {

	//! Environment desriptor
	/*! Environment desriptor class
	*/
	class environmentDescriptor
	{
	private:
		int m, n;  //!< field size
		vector<vector<Point>> field;;  //!< field 
		vector<Components_N::Component> components; ;  //!< used components
		vector<Point> pointsOfInterest;  //!< arary of points of interest
	public:
		//! Constructor
		environmentDescriptor(int m0 = 0, int n0 = 0) :m(m0), n(n0) {};
		//! Destructor
		~environmentDescriptor() { 
			field.~vector(); 
			components.~vector();
			pointsOfInterest.~vector();
		};
		//!< get size of field
		int getSize() {};
		//!< set size of field
		void setSize(int m, int n) {};
		//!< get type of cell
		int getCell(Point point) {};
		//!< set type of cell
		void setCell(Point point) {};
	};
}

#endif