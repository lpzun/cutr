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

extern vertex INITL_V;
extern vertex FINAL_V;

extern vertex INITL_SCC;
extern vertex FINAL_SCC;

typedef unordered_map<vertex, list<vertex>> adj_list;
typedef list<vertex> _path;
typedef unsigned int id_scc;

class Graph {
public:

	Graph(const size_V& V, const adj_list& Adj);
	virtual ~Graph();
	void build_SCC();

	vertex get_sccs_size() const;
	const vector<list<vertex> >& get_sccs() const;
	vector<_path> find_all_paths(const vertex& start, const vertex& final);

private:
	size_V V;        /// No. of vertices
	adj_list Adj;    /// adjacency list

	id_scc scc_id;
	vector<list<vertex>> sccs;

	void DFS();
	void DFS_visit(const vertex& u, vector<bool>& visited, const adj_list& Adj);
	void sort(const vertex& u, vector<bool>& visited, stack<vertex>& sstack);
	adj_list transpose();

	/// find all paths
	void find_all_paths(const vertex& final, _path& visited,
			vector<_path>& paths);
	void print_path(const _path& path);
};

} /* namespace SURA */

#endif /* GRAPH_HH_ */
