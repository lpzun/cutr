/**
 *  @brief test.cc
 *  @date  : Jun 12, 2015
 *  @author: Peizun Liu
 */
#include "../util/graph.hh"

using namespace sura;

int test_main(const int argc, const char * const * const argv) {
	adj_list Adj;

	Adj[1].push_back(2);
	Adj[1].push_back(3);

	Adj[2].push_back(1);
	Adj[2].push_back(4);
	Adj[2].push_back(5);
	Adj[2].push_back(6);

	Adj[3].push_back(1);
	Adj[3].push_back(5);
	Adj[3].push_back(6);

	Adj[4].push_back(2);

	Adj[5].push_back(3);
	Adj[5].push_back(6);

	Adj[6].push_back(2);
	Adj[6].push_back(3);
	Adj[6].push_back(5);

	Graph g(6, Adj);
	g.find_all_paths(2, 5);

	return 0;

}

