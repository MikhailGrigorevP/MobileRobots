#ifndef _ENVIRONMENTDESCRIPTOR_H_
#define _ENVIRONMENTDESCRIPTOR_H_

#include "Component.h"

namespace ED_N {

	struct Point {
		int x = 0;
		int y = 0;
	};

	class environmentDescriptor
	{
	private:
		int m, n;
		int** field;
		Components_N::Component* components;
		Point* points;

	public:
		int getSize() {};
		void setSize() {};
		int getCell() {};
		void setCell() {};
	};
}

#endif