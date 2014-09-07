#include "path_finding.hpp"

inline int heuristic(Pair<int> a, Pair<int> b) {
  return a.manhattan_dist(b);
}

void a_star_search
  (
	void (*get_map_neighbors)( Pair<int> ), // pointer to static member that gives us neighbors
   Pair<int> start,
   Pair<int> goal,
   unordered_map< Pair<int>, Pair<int> >& came_from,
   unordered_map< Pair<int>, int >& cost_so_far)
{
  PriorityQueue< Pair<int> > frontier;
  frontier.put(start, 0);

  came_from[start] = start;
  cost_so_far[start] = 0;

  while (!frontier.empty()) {
    Pair<int> current = frontier.get();

    if (current == goal) {
      break;
    }

    for (Pair<int> next : (*get_map_neighbors)(current)) {
    	/* this number 1 is the cost to move - no weighting scheme is being used right now
    	 */
      int new_cost = cost_so_far[current] + 1;

      if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
        cost_so_far[next] = new_cost;
        int priority = new_cost + heuristic(next, goal);
        frontier.put(next, priority);
        came_from[next] = current;
      }
    }
  }
}
