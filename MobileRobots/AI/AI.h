#ifndef _AI_H_
#define _AI_H_

#include "../Structures/structures.h"

#include "../environmentDescriptor/environmentDescriptor.h"
using namespace my_std;
//using namespace std;

//! Artificial intelligence namespace
/*! AI namespace
*/
namespace AI_N {

	/** \defgroup groupSearchingPoints Searching points of interest
	 * \brief AI is used to find all points of interest.
	 *
	 * With this class, following functions can be done:
	 * - findInterestPoints
	 * @{
	 */
	//! Artificial intelligence
	/*! AI class
 	*/
	class AI
	{
	private:
		vector<Point> interestPonts; //!< Array with all points of interest
	public:
		//! Constructor
		AI() {};
		//! Destructor
		~AI() { interestPonts.~vector(); };

		vector<Point> findInterestPoints(ED_N::environmentDescriptor* environment);
	};
	/** @} */

}
#endif

