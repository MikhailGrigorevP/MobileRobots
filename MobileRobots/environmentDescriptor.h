#ifndef _ENVIRONMENTDESCRIPTOR_H_
#define _ENVIRONMENTDESCRIPTOR_H_

#include "Component.h"

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
		int** field;;  //!< field 
		Components_N::Component* components; ;  //!< used components
		Point* pointsOfInterest;  //!< arary of points of interest
	public:
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