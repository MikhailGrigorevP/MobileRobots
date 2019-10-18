#ifndef _AI_H_
#define _AI_H_

#include "structures.h"

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
	public:
		//!Constructor
		AI() {};
		//!Destructor
		~AI() {};
		//!Method that returns all points of intererst
		Point* findInterestPoints() {};
	};
	/** @} */

}
#endif

