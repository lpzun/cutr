/*
 * Graph.cc
 *
 *  Created on: Jun 23, 2015
 *      Author: lpzun
 */

#include "graph.hh"

namespace sura {

vertex Graph::delegate = 0;
map<vertex, list<vertex>> Graph::sccs;

Graph::Graph(const size_V& V, const adj_list& Adj) :
		V(V), Adj(Adj) {
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
			delegate = u;
			cout << "------------" << delegate << endl;
			sccs.insert(
					std::pair<vertex, list<vertex>>(delegate, list<vertex>()));
			trsp_G.DFS_visit(u, visited);
			cout << endl; // delete ------------
		}
	}
}

void Graph::DFS() {
	vector<bool> visited(V, false);
	for (auto u = 0; u < V; u++) {
		if (!visited[u])
			DFS_visit(u, visited);
	}
}

/**
 * @brief a recursive function to do DFS starting from u
 * @param u: starting point and the root of a DFS tree
 * @param visited: to mark if a node is visited or not
 */
void Graph::DFS_visit(const vertex& u, vector<bool>& visited) {
	visited[u] = true;
	cout << u << " "; // delete this -------
	sccs[delegate].push_back(u);
	for (auto iv = Adj[u].begin(); iv != Adj[u].end(); iv++) {
		if (!visited[*iv])
			DFS_visit(*iv, visited);
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
	sstack.push(u);
}

/**
 * @brief compute transpose
 * @return transpose of a graph
 */
Graph Graph::transpose() {
	adj_list trps_G; /// transpose of graph
	for (auto u = 0; u < V; u++) {
		for (auto iv = Adj[u].begin(); iv != Adj[u].end(); iv++) {
			trps_G[*iv].push_back(u);
		}
	}
	return Graph(V, trps_G);
}

} /* namespace SURA */
