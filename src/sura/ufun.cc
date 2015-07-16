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
delta Ufun::compute_delta(const list<edge>& ifx) {
	delta d;
	for (auto itran = ifx.begin(); itran != ifx.end(); ++itran) {
		if (itran->get_type() != type_T::EXPD)
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
	if (e.get_type() != type_T::THCR)
		d[mapping_TS[e.get_src()].local]--;
	d[mapping_TS[e.get_dst()].local]++;
}

/**
 * @brief determine if (src, dst) corresponds to a spawn transition
 * @param src
 * @param dst
 * @return bool
 * 			true : if src +> dst
 * 			false: otherwise
 */
bool Ufun::is_spawn_transition(const Thread_State& src,
		const Thread_State& dst) {
	// TODO: determine if src +> dst is true
	return false;
}

/**
 * @brief extract all edges from <en> to <ex> in a simple loop
 * @param scc: a simple loop
 * @param en : enter point
 * @param ex : exit  point
 * @return list<edge>
 * 		all real edges from <en> to <ex>
 */
list<edge> Ufun::extract_trans_enter_to_exit(const SCC& scc, const vertex& en,
		const vertex& ex) {
	list<edge> en_to_ex;

	vector<vertex> Adj(scc.size());
	for (auto itran = scc.get_E().begin(); itran != scc.get_E().end();
			++itran) {
		Adj[itran->get_src()] = itran->get_dst();
	}

	auto next = en;
	while (next != ex) {
		if (ETTD::real_R[next][Adj[next]])
			en_to_ex.push_back(edge(next, Adj[next]));
		next = Adj[next];
	}

	return en_to_ex;
}

} /* namespace sura */
