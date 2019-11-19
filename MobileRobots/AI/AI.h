#ifndef _AI_H_
#define _AI_H_
#include <map>
#include <stack>
#include <queue>
#include "../Structures/structures.h"

using namespace my_std;
using std::queue;
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
		std::map<Point, vector<Vertex>> _graph;
		vector<vector<int>> visited_vertex;
		vector<Point> interestPonts; //!< Array with all points of interest
	public:
		//! Constructor
		AI() {};
		//! Destructor
		~AI() { interestPonts.~vector(); };

		void bfs(Point start, Point e, vector<vector<unsigned>>& field);
		vector<Point> findInterestPoints(ED_N::environmentDescriptor* environment);
		void dfs(ED_N::environmentDescriptor* environment, vector<vector<unsigned>>& field,  managementComponent* component);
	};
	/** @} */

	


}
#endif

