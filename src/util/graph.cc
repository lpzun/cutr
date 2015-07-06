/*
 * Graph.cc
 *
 *  Created on: Jun 23, 2015
 *      Author: lpzun
 */

#include "graph.hh"

namespace sura {

Graph::Graph(const size_V& V, const adj_list& Adj) :
		V(V), Adj(Adj) {
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

void Graph::build_SCC() {
	stack<vertex> sstack;
	vector<bool> visited(V, false);

	for (auto u = 0; u < V; u++) {
		if (!visited[u])
			this->sort(u, visited, sstack);
	}

	auto trsp_G = this->transpose();

	std::fill(visited.begin(), visited.end(), false);

	while (!sstack.empty()) {
		auto u = sstack.top();
		sstack.pop();

		if (!visited[u])
			trsp_G.DFS_visit(u, visited);
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
void Graph::sort(const vertex& u, vector<bool>& visited, stack<vertex>& sstack) {
	visited[u] = true;
	for (auto iv = Adj[u].begin(); iv != Adj[u].end(); iv++) {
		if (!visited[*iv])
			sort(*iv, visited, sstack);
	}
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
