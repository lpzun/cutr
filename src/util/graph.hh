/*
 * Graph.hh
 *
 *  Created on: Jun 23, 2015
 *      Author: lpzun
 */

#ifndef GRAPH_HH_
#define GRAPH_HH_

#include "utilities.hh"

namespace SURA {

typedef unsigned int vertex;
typedef unordered_map<vertex, list<vertex> > adj_list;

class Graph {
public:
	Graph();
	virtual ~Graph();

	void DFS();
	void DFS(const list<vertex>& V);
	void DFS(const adj_list& G, const vertex& u);

	adj_list transpose();
	void strongly_connected_component();

private:
	adj_list Adj;
	list<vertex> V;
};

} /* namespace SURA */

#endif /* GRAPH_HH_ */
