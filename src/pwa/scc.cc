/**
 * @brief scc.cc
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#include "scc.hh"

namespace sura {

GSCC::GSCC(const size_V& V, const adj_list& Adj) :
		sccs(), trans_btwn_sccs(), paths() {
	const auto& sscc = this->build_SCC(V, Adj);
	this->sccs = vector<shared_ptr<SCC>>(sscc.size(), nullptr);
	this->trans_btwn_sccs = vector<vector<shared_ptr<deque<edge>>> >(sscc.size(),
	vector<shared_ptr<deque<edge>>>(sscc.size(), nullptr));
	this->build_GSCC(sscc);
}

GSCC::~GSCC() {
}

/**
 * @brief compute all SCCs in expanded TTD
 * @param V  : the set of vertices
 * @param Adj: adjacent list to represent expanded TTD
 */
vector<deque<vertex>> GSCC::build_SCC(const size_V& V, const adj_list& Adj) {
	Graph g(V, Adj);
	g.build_SCC();

#ifndef NDEBUG
	cout << __func__ << "\n";
	for (size_t idx = 0; idx < g.get_sccs().size(); ++idx) {
		cout << idx << " size= " << g.get_sccs()[idx].size() << " ";
		for (auto iv = g.get_sccs()[idx].begin(); iv != g.get_sccs()[idx].end();
				++iv)
			cout << Refs::mapping_TS[*iv] << " ";
		cout << endl;
	}
#endif

	return g.get_sccs();
}

/**
 * @brief build a GSCC graph
 * @param sccs
 */
void GSCC::build_GSCC(const vector<deque<vertex>>& sccs) {
	/// construct SCCs: vertices of SCC quotient graph
	for (size_t idx = 0; idx < sccs.size(); ++idx) {
		if (sccs[idx].size() > 1)
			this->sccs[idx] = std::make_shared<SCC>(sccs[idx].front(),
					sccs[idx]);
		else
			this->sccs[idx] = std::make_shared<SCC>(sccs[idx].front());
	}

	/// construct adjacency list of SCC quotient graph: id_scc
	adj_list Adj;
	for (size_t iu = 0; iu < sccs.size(); ++iu) {
		for (size_t iv = iu + 1; iv < sccs.size(); ++iv) {
			bool is_uv = false, is_vu = false;
			this->build_E_in_GSCC(sccs[iu], iu, sccs[iv], iv, is_uv, is_vu);
			if (is_uv)
				Adj[iu].emplace_back(iv);
			else if (is_uv)
				Adj[iv].emplace_back(iu);
		}
	}

#ifndef NDEBUG
	cout << __func__ << " The SCC quotient graph:\n";
	for (auto isrc = Adj.begin(); isrc != Adj.end(); ++isrc) {
		for (auto idst = isrc->second.begin(); idst != isrc->second.end();
				++idst) {
			cout << Refs::mapping_TS[this->sccs[isrc->first]->get_v()] << " -> "
					<< Refs::mapping_TS[this->sccs[*idst]->get_v()] << " ";
			cout << isrc->first << " -> " << *idst << "\n";
		}
	}

	cout << __func__ << " Transitions between SCCs:\n";
	for (size_t i = 0; i < this->trans_btwn_sccs.size(); ++i) {
		for (size_t j = 0; j < this->trans_btwn_sccs[0].size(); ++j) {
			if (trans_btwn_sccs[i][j] != nullptr) {
				for (const auto &t : *(trans_btwn_sccs[i][j]))
					cout << t << " ";
				cout << "\n";
			}
		}
	}
	cout << endl;
#endif
	Graph g(sccs.size(), Adj);
	paths = g.find_all_paths(INITL_SCC, FINAL_SCC);
}

/**
 * @brief  compute all SCC quotient paths in SCC quotient graph
 * @return a list of quotient paths
 */
const vector<_path>& GSCC::find_all_paths() const {
	return this->paths;
}

/**
 * @brief pass reference version
 * @param scc1
 * @param u
 * @param scc2
 * @param v
 * @param is_uv
 * @param is_vu
 */
void GSCC::build_E_in_GSCC(const deque<vertex>& scc1, const size_t& u,
		const deque<vertex>& scc2, const size_t& v, bool& is_uv, bool& is_vu) {
	deque<edge> UV, VU;
	for (auto iu = scc1.begin(); iu != scc1.end(); ++iu) {
		for (auto iv = scc2.begin(); iv != scc2.end(); ++iv) {
			if (ETTD::real_R[*iu][*iv]) {
				is_uv = true;
				UV.emplace_back(*iu, *iv);
			} else if (ETTD::expd_R[*iu][*iv]) {
				is_uv = true;
				UV.emplace_back(*iu, *iv);
			}

			if (ETTD::real_R[*iv][*iu]) {
				is_vu = true;
				VU.emplace_back(*iv, *iu);
			} else if (ETTD::expd_R[*iv][*iu]) {
				is_vu = true;
				VU.emplace_back(*iv, *iu);
			}
		}
	}

	if (is_uv && is_vu)
		throw ural_rt_err("SCC computation is wrong!");
	if (is_uv)
		this->trans_btwn_sccs[u][v] = std::make_shared<deque<edge>>(UV);
	else if (is_vu)
		this->trans_btwn_sccs[v][u] = std::make_shared<deque<edge>>(VU);
}

/////////////////////////// SCC class /////////////////////////////////////////

/**
 * @brief constructor
 * @param v
 */
SCC::SCC(const vertex& v) :
		is_TRIVIAL(true), is_NESTED(false), V_size(0), v(v) {
}

/**
 * @brief constructor with delegate v and vertices V
 * @param v: delegate
 * @param V: the set of vertices
 */
SCC::SCC(const vertex& v, const deque<vertex>& V) :
		is_TRIVIAL(false), is_NESTED(false), V_size(V.size()), v(v), E() {
	this->is_NESTED = this->is_loop_nests(this->V_size, this->build_E(V));
}

SCC::~SCC() {
}

bool SCC::is_trivial() const {
	return is_TRIVIAL;
}

bool SCC::is_nested() const {
	return is_NESTED;
}

const vertex& SCC::get_v() const {
	return v;
}

const deque<edge>& SCC::get_E() const {
	return E;
}

const size_V SCC::size() const {
	return V_size;
}

/**
 * @brief build the set of egdes E
 * @param V
 */
uint SCC::build_E(const deque<vertex>& V) {
	uint size = 0;
	for (auto iu = V.begin(); iu != V.end(); ++iu) {
		for (auto iv = V.begin(); iv != V.end(); ++iv) {
			if (*iu != *iv && ETTD::real_R[*iu][*iv]) {
				E.emplace_back(*iu, *iv, Transition::ID++);
				size++;
			} else if (*iu != *iv && ETTD::expd_R[*iu][*iv]) {
				size++;
			}
		}
	}
	return size;
}

/**
 * @brief determine if SCC is loop nests
 * @param size
 * @param size
 * @return bool
 * 			true : if SCC is loop nests
 * 			false: otherwise
 */
bool SCC::is_loop_nests(const size_V& size, const uint& size_E) {
	return size_E > size;
}

} /* namespace SURA */
