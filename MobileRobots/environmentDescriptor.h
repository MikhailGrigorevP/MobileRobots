#ifndef _ENVIRONMENTDESCRIPTOR_H_
#define _ENVIRONMENTDESCRIPTOR_H_

#include "Component.h"
using std::vector;
using namespace Components_N;
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
		Field_size size;  //!< field size
		vector<vector<unsigned>> field;  //!< field 
		vector<Component*> components;  //!< used components
	
	public:


		robotCommander* getRCComponent(int i) {
			return dynamic_cast<robotCommander*>(components[i]);
		}


		managementComponent* getOCComponent(int i) {
			return dynamic_cast<managementComponent*>(components[i]);
		}

		robotScout* getRSComponent(int i) {
			return dynamic_cast<robotScout*>(components[i]);
		}
		
		Component* getComponent(int i) {
			return components[i];
		}


		environmentDescriptor(Field_size field_size);
		
		environmentDescriptor(int m0 = 0, int n0 = 0);
		
		~environmentDescriptor();
		
		int getCompCount() { return components.size(); };

		Field_size getSize();
		
		void setSize(Field_size field_size);
	
		void drawMap();

		void showComponents();

		void AddObserveCenter(Point point,  int en, int num, int c);

		void AddCommandCenter(Point point, int en, int num, int numD, int c);

		void AddRobotCommander(Point point, int en, int num, int numD, int vel, int c);

		void AddRobotScout(Point point, int en, int num, int vel, int c);
		
		unsigned getCell(Point point);
		
		void setCell(Point point, unsigned cell_type);

		// Input
		friend std::istream& operator >> (std::istream& s, environmentDescriptor& envD);

		// Output
		friend std::ostream& operator << (std::ostream& o, const environmentDescriptor& envD);
	};
}

#endif