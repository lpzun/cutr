/*
 * Graph.cc
 *
 *  Created on: Jun 23, 2015
 *      Author: lpzun
 */

#include "graph.hh"

namespace sura {

vertex INITL_V = 0;
vertex FINAL_V = 0;

vertex INITL_SCC = 0;
vertex FINAL_SCC = 0;

Graph::Graph(const size_V& V, const adj_list& Adj) :
		V(V), Adj(Adj), scc_id(0), sccs() {
	this->sccs.reserve(V);
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

void Graph::build_SCC() {
	stack<vertex> sstack;

	///  mark all vertices as not visited (for first DFS)
	vector<bool> visited(V, false);

	/// fill vertices in stack according to their finishing time
	for (auto u = 0; u < V; u++) {
		if (!visited[u])
			this->sort(u, visited, sstack);
	}

	/// create a reversed graph
	auto trsp_G = this->transpose();

	/// reset all vertices as not visited (for second DFS)
	std::fill(visited.begin(), visited.end(), false);

	/// process all vertices in order defined by stack
	while (!sstack.empty()) {
		/// pop a vertex from stack
		auto u = sstack.top();
		sstack.pop();

		/// print strongly connected component of the popped vertex
		if (!visited[u]) {
			sccs.emplace_back(list<vertex>());
			DFS_visit(u, visited, trsp_G);
			scc_id++;
			cout << __func__ << "------------" << scc_id << endl; // delete ------------
		}
	}
}

void Graph::DFS() {
	vector<bool> visited(V, false);
	for (auto u = 0; u < V; u++) {
		if (!visited[u])
			DFS_visit(u, visited, Adj);
	}
}

/**
 * @brief a recursive function to do DFS starting from u
 * @param u: starting point and the root of a DFS tree
 * @param visited: to mark if a node is visited or not
 */
void Graph::DFS_visit(const vertex& u, vector<bool>& visited,
		const adj_list& Adj) {
	visited[u] = true;
	cout << u << " "; // delete ------------
	if (INITL_V == u)
		INITL_SCC = scc_id;
	if (FINAL_V == u)
		FINAL_SCC = scc_id;
	sccs[scc_id].emplace_back(u);
	auto ifind = Adj.find(u);
	if (ifind != Adj.end())
		for (auto iv = ifind->second.begin(); iv != ifind->second.end(); ++iv) {
			if (!visited[*iv])
				DFS_visit(*iv, visited, Adj);
		}
}

/**
 * @brief Fills sstack with vertices (in increasing order of finishing time).
 * 		  The top element of stack has the maximum finishing time
 * @param u:
 * @param visited: starting point and the root of a DFS tree
 * @param sstack : store vertices in increasing order of finishing time
 */
void Graph::sort(const vertex& u, vector<bool>& visited,
		stack<vertex>& sstack) {
	/// mark the current node as visited and print it
	visited[u] = true;
	/// recur for all vertices adjacent to this vertex
	for (auto iv = Adj[u].begin(); iv != Adj[u].end(); iv++) {
		if (!visited[*iv])
			sort(*iv, visited, sstack);
	}
	/// all vertices reachable from u are processed by now
	sstack.emplace(u);
}

/**
 * @brief compute transpose
 * @return transpose of a graph
 */
adj_list Graph::transpose() {
	adj_list trps_G; /// transpose of graph
	for (auto u = 0; u < V; u++) {
		for (auto iv = Adj[u].begin(); iv != Adj[u].end(); iv++) {
			trps_G[*iv].emplace_back(u);
		}
	}
	return trps_G;
}

/**
 * @brief compute all paths from start to final
 * @param start: starting point
 * @param final: ending   point
 * @return all paths
 */
vector<_path> Graph::find_all_paths(const vertex& start, const vertex& final) {
	vector<_path> paths;
	_path visited;
	visited.emplace_back(start);
	this->find_all_paths(final, visited, paths);
	// testing print delete ----------------------
	for (const auto& path : paths)
		this->print_path(path);
	return paths;
}

/**
 * @brief recursive version
 * @param final  : ending   point
 * @param visited:
 * @param paths  : the list of paths
 * @return all paths
 */
void Graph::find_all_paths(const vertex& final, _path& visited,
		vector<_path>& paths) {
	auto ifind = this->Adj.find(visited.back());
	if (ifind != Adj.end()) {
		for (auto inhb = ifind->second.begin(); inhb != ifind->second.end();
				++inhb) {
			if (std::find(visited.begin(), visited.end(), *inhb)
					!= visited.end())
				continue;

			if (*inhb == final) {
				cout << *inhb << endl;
				visited.emplace_back(*inhb);
				paths.emplace_back(visited);
				visited.pop_back();
				break;
			}
		}

		for (auto inhb = ifind->second.begin(); inhb != ifind->second.end();
				++inhb) {
			if (*inhb == final
					|| std::find(visited.begin(), visited.end(), *inhb)
							!= visited.end())
				continue;

			visited.emplace_back(*inhb);
			find_all_paths(final, visited, paths);
			visited.pop_back();
		}
	}
}

void Graph::print_path(const _path& path) {
	for (auto iv = path.begin(); iv != path.end(); ++iv) {
		cout << *iv << " ";
	}
	cout << endl;
}

vertex Graph::get_sccs_size() const {
	return scc_id - 1;
}

const vector<list<vertex> >& Graph::get_sccs() const {
	return sccs;
}

} /* namespace SURA */
