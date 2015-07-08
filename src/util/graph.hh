/*
 * Graph.hh
 *
 *  Created on: Jun 23, 2015
 *      Author: lpzun
 */

#ifndef GRAPH_HH_
#define GRAPH_HH_

#include "heads.hh"

namespace sura {

typedef unsigned int vertex;
typedef unsigned int size_V;

typedef unordered_map<vertex, list<vertex>> adj_list;

class Graph {
public:
	static vertex delegate;
	static map<vertex, list<vertex>> sccs;

	Graph(const size_V& V, const adj_list& Adj);
	virtual ~Graph();
	void build_SCC();
private:
	size_V V;        /// No. of vertices
	adj_list Adj;    /// adjacency list

	void DFS();
	void DFS_visit(const vertex& u, vector<bool>& visited);
	void sort(const vertex& u, vector<bool>& visited, stack<vertex>& sstack);
	Graph transpose();
};

} /* namespace SURA */

#endif /* GRAPH_HH_ */
