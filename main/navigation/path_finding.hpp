#ifndef PATH_FINDING_HPP
#define PATH_FINDING_HPP


#include "../helpers/containers.hpp"
#include <unordered_map>


inline int heuristic(Pair<int> a, Pair<int> b);

void a_star_search
  (
   vector<Pair<int> > (*get_map_neighbors)( Pair<int> ), // pointer to static member that gives us neighbors
   Pair<int> start,
   Pair<int> goal,
   unordered_map< Pair<int>, Pair<int> >& came_from,
   unordered_map< Pair<int>, int >& cost_so_far
);


#endif
