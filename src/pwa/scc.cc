/**
 * @brief scc.cc
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#include "scc.hh"

namespace sura {

GSCC::GSCC() {
}

GSCC::~GSCC() {
}

/**
 * @brief compute all SCCs in expanded TTD
 * @param V  : the set of vertices
 * @param Adj: adjacent list to represent expanded TTD
 */
map<vertex, list<vertex>> GSCC::build_SCC(const size_V& V,
		const adj_list& Adj) {
	Graph g(V, Adj);
	g.build_SCC();

	// delete ---------------------------------------------------------------
	for (auto idlg = Graph::sccs.begin(); idlg != Graph::sccs.end(); ++idlg) {
		cout << idlg->first << " size=" << idlg->second.size() << " ";
		for (auto iv = idlg->second.begin(); iv != idlg->second.end(); ++iv) {
			cout << mapping_TS[*iv] << " ";
		}
		cout << endl;
	}

	return Graph::sccs;
}

/**
 * @brief build a GSCC graph
 * @param sccs
 */
void GSCC::build_GSCC(const map<vertex, list<vertex>>& sccs) {
	map<vertex, SCC> map_V_SCC;
	for (auto iscc = sccs.begin(); iscc != sccs.end(); ++iscc) {
		if (iscc->second.size() > 1)
			map_V_SCC.insert(
					std::pair<vertex, SCC>(iscc->first, SCC(iscc->first)));
		else
			map_V_SCC.insert(
					std::pair<vertex, SCC>(iscc->first,
							SCC(iscc->first, iscc->second)));
	}

	adj_list Adj;
	auto iu = sccs.begin();
	while (iu != sccs.end() && std::next(iu) != sccs.end()) {
		for (auto iv = std::next(iu); iv != sccs.end(); ++iv) {
			bool is_uv = false, is_vu = false;
			this->build_E_in_GSCC(iu->second, iv->second, is_uv, is_vu);
			if (is_uv)
				Adj[iu->first].push_back(iv->first);
			else if (is_uv)
				Adj[iv->first].push_back(iu->first);
		}
		++iu;
	}

#ifndef NDEBUG
	cout << "The SCC quotient graph:" << endl;
	for (auto isrc = Adj.begin(); isrc != Adj.end(); ++isrc) {
		for (auto idst = isrc->second.begin(); idst != isrc->second.end();
				++idst) {
			cout << mapping_TS[isrc->first] << " -> " << mapping_TS[*idst]
					<< " ";
			cout << isrc->first << " -> " << *idst << "\n";
		}
	}
#endif

	Graph g(mapping_TS.size(), Adj);
	cout << INITL_V << " $$$$ " << FINAL_V << endl;
	auto paths = g.find_all_paths(INITL_SCC, FINAL_SCC);
	cout << endl;
}

/**
 * @brief  compute all SCC quotient paths in SCC quotient graph
 * @return a list of quotient paths
 */
vector<_path> GSCC::find_all_paths() {
	vector<_path> paths;
	return paths;
}

/**
 * @brief pass by reference version
 * @param scc1
 * @param scc2
 */
void GSCC::build_E_in_GSCC(const list<vertex>& scc1, const list<vertex>& scc2,
		bool& is_uv, bool& is_vu) {
	list<edge> UV, VU;
	for (auto iu = scc1.begin(); iu != scc1.end(); ++iu) {
		for (auto iv = scc2.begin(); iv != scc2.end(); ++iv) {
			if (ETTD::real_R[*iu][*iv]) {
				is_uv = true;
				UV.push_back(edge(*iu, *iv, type_T::NORM));
			} else if (ETTD::expd_R[*iu][*iv]) {
				is_uv = true;
				UV.push_back(edge(*iu, *iv, type_T::EXPD));
			}

			if (ETTD::real_R[*iv][*iu]) {
				is_vu = true;
				VU.push_back(edge(*iv, *iu, type_T::NORM));
			} else if (ETTD::expd_R[*iv][*iu]) {
				is_vu = true;
				VU.push_back(edge(*iv, *iu, type_T::EXPD));
			}
		}
	}

	if (is_uv && is_vu)
		throw ural_rt_err("SCC computation is wrong!");
}

/**
 * @brief pass by iterator version
 * @param scc1
 * @param scc2
 */
void GSCC::build_E_in_GSCC(map<vertex, list<vertex>>::const_iterator scc1,
		map<vertex, list<vertex>>::const_iterator scc2) {
	bool is_uv = false, is_vu = false;
	list<edge> UV, VU;
	for (auto iu = scc1->second.begin(); iu != scc1->second.end(); ++iu) {
		for (auto iv = scc2->second.begin(); iv != scc2->second.end(); ++iv) {
			if (ETTD::real_R[*iu][*iv]) {
				is_uv = true;
				UV.push_back(edge(*iu, *iv, type_T::NORM));
			} else if (ETTD::expd_R[*iu][*iv]) {
				is_uv = true;
				UV.push_back(edge(*iu, *iv, type_T::EXPD));
			}

			if (ETTD::real_R[*iv][*iu]) {
				is_vu = true;
				VU.push_back(edge(*iv, *iu, type_T::NORM));
			} else if (ETTD::expd_R[*iv][*iu]) {
				is_vu = true;
				VU.push_back(edge(*iv, *iu, type_T::EXPD));
			}
		}
	}

	if (is_uv && is_vu)
		throw ural_rt_err("SCC computation is wrong!");

	if (is_uv)
		for (const auto& r : UV)
			cout << r << endl;
	else if (is_uv)
		for (const auto& r : VU)
			cout << r << endl;
}

/////////////////////////// SCC class /////////////////////////////////////////

SCC::SCC(const vertex& v) :
		v(v), is_TRIVIAL(true), is_NESTED(false), V_size(0) {
}

/**
 * @brief constructor with delegate v and vertices V
 * @param v: delegate
 * @param V: the set of vertices
 */
SCC::SCC(const vertex& v, const list<vertex>& V) :
		v(v), is_TRIVIAL(false) {
	this->V_size = V.size();
	this->build_E(V);
	this->is_NESTED = this->is_loop_nests(this->V_size);
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

const list<edge>& SCC::get_E() const {
	return E;
}

const size_V SCC::size() const {
	return V_size;
}

/**
 * @brief build the set of egdes E
 * @param V
 */
void SCC::build_E(const list<vertex>& V) {
	if (this->is_NESTED) {
		for (auto iu = V.begin(); iu != V.end(); ++iu) {
			for (auto iv = V.begin(); iv != V.end(); ++iv) {
				if (*iu != *iv && ETTD::real_R[*iu][*iv])
					E.push_back(edge(*iu, *iv));
				/// TODO: create edges with id
				/// E.push_back(edge(*iu, *iv, Transition::ID++));
			}
		}
	} else { /// for a simple loop, we includes expansion edges
		for (auto iu = V.begin(); iu != V.end(); ++iu) {
			for (auto iv = V.begin(); iv != V.end(); ++iv) {
				if (*iu != *iv && ETTD::real_R[*iu][*iv])
					E.push_back(edge(*iu, *iv));
				else if (*iu != *iv && ETTD::real_R[*iu][*iv])
					E.push_back(edge(*iu, *iv, type_T::EXPD));
				/// TODO: create edges with id
				/// E.push_back(edge(*iu, *iv, Transition::ID++));
			}
		}
	}
}

/**
 * @brief determine if SCC is loop nests
 * @param size
 * @return bool
 * 			true : if SCC is loop nests
 * 			false: otherwise
 */
bool SCC::is_loop_nests(const size_V& size) {
	if (E.size() > size)
		return true;
	return false;
}

} /* namespace SURA */
