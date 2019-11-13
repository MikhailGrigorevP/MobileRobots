#ifndef _AI_H_
#define _AI_H_
#include <map>
#include <stack>
#include "../Structures/structures.h"

using namespace my_std;
//using std::vector;
#include "../environmentDescriptor/environmentDescriptor.h"


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
		std::map<Vertex, vector<Vertex>> _graph;
		vector<vector<bool>> visited_vertex;
		vector<Point> interestPonts; //!< Array with all points of interest
	public:
		//! Constructor
		AI() {};
		//! Destructor
		~AI() { interestPonts.~vector(); };

		vector<Point> findInterestPoints(ED_N::environmentDescriptor* environment);
		void dfs(ED_N::environmentDescriptor* environment, vector<vector<unsigned>>& field,  managementComponent* component);
	};
	/** @} */

}
#endif

