/**
 * ufun.cc
 *
 *  Created on: Jul 12, 2015
 *      Author: lpzun
 */

#include "ufun.hh"

namespace sura {

Ufun::Ufun() {
	// TODO Auto-generated constructor stub

}

Ufun::~Ufun() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief compute the delta of local states in infix
 * @param ifx: a linear path or a simple loop
 * @return delta:
 * 		the distance for a linear path or a simple loop
 */
delta Ufun::compute_delta(const deque<edge>& ifx) {
	delta d;
	for (auto itran = ifx.begin(); itran != ifx.end(); ++itran) {
		if (ETTD::real_R[itran->get_src()][itran->get_dst()])
			update_counter(d, *itran);
	}
	return d;
}

/**
 * @brief compute the delta of local states for an edge
 * @param ifx: a linear path or a simple loop
 * @return delta:
 * 		the distance for an edge
 */
delta Ufun::compute_delta(const edge& ifx) {
	delta d;
	update_counter(d, ifx);
	return d;
}

/**
 * @brief update counter: call by functions to compute deltas
 * @param d: delta
 * @param e: an edge
 */
void Ufun::update_counter(delta &d, const edge &e) {
	d[Refs::mapping_TS[e.get_src()].get_local()]--;
	d[Refs::mapping_TS[e.get_dst()].get_local()]++;
}

/**
 * @brief extract all edges from <en> to <ex> in a simple loop
 * @param scc: a simple loop
 * @param en : enter point
 * @param ex : exit  point
 * @return list<edge>
 * 		all real edges from <en> to <ex>
 */
deque<edge> Ufun::extract_trans_enter_to_exit(const deque<edge>& E,
		const vertex& en, const vertex& ex) {
	deque<edge> en_to_ex;

	set<vertex> V;
	for (auto itran = E.begin(); itran != E.end(); ++itran) {
		V.emplace(itran->get_src());
		V.emplace(itran->get_dst());
	}

	map<vertex, vertex> Adj;
	for (auto iu = V.begin(); iu != V.end(); ++iu) {
		for (auto iv = V.begin(); iv != V.end(); ++iv) {
			if (ETTD::real_R[*iu][*iv] || ETTD::expd_R[*iu][*iv])
				Adj.emplace(*iu, *iv);
		}
	}

	auto next = en;
	while (next != ex) {
		if (ETTD::real_R[next][Adj[next]])
			en_to_ex.emplace_back(edge(next, Adj[next]));
		next = Adj[next];
	}

#ifndef NDEBUG
	cout << __func__ << "\n";
	for (auto &t : en_to_ex)
	cout << t << "\n";
#endif

	return en_to_ex;
}

} /* namespace sura */
