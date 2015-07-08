/**
 * @brief: ettd.cc
 * @date : Jun 21, 2015
 * @author: Peizun Liu
 */

#include "ettd.hh"

namespace sura {

/// Global static pointer used to ensure a single instance of the class.
unique_ptr<ETTD> ETTD::m_instance = nullptr;

/**
 * @brief This function is called to create an instance of the class.
 * 		  Calling the constructor publicly is not allowed. The constructor
 * 		  is private and is only called by this instance function.
 *
 * @return the reference of ETTD
 */
ETTD& ETTD::instance() {
	if (!m_instance)
		m_instance.reset(
				std::unique_ptr<ETTD>(new ETTD(original_TTD)).release());
	return *m_instance;
}

ETTD::ETTD() {
	// TODO Auto-generated constructor stub
}

ETTD::ETTD(const adj_list& TTD) {
	this->build_ETTD(TTD);
}

ETTD::~ETTD() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief build expanded TTD
 * @param TTD: original TTD, represented by adjacency list
 */
void ETTD::build_ETTD(const adj_list& TTD) {
	this->expanded_TTD = TTD; /// copy the original TTD
	auto vfind = s_incoming.begin();
	auto wfind = s_outgoing.begin();
	for (auto s = 0; s < Thread_State::S; ++s) {
		vfind = s_incoming.find(s);
		wfind = s_outgoing.find(s);
		if (vfind != s_incoming.end() && wfind != s_outgoing.end())
			for (auto iv = vfind->second.begin(); iv != vfind->second.end();
					++iv)
				for (auto iw = wfind->second.begin(); iw != wfind->second.end();
						++iw)
					if (*iv != *iw)
						if (!this->is_real_trans(*iv, *iw, TTD)) {
							expanded_TTD[*iv].push_back(*iw); /// add expansion edge
							transitions.push_back(edge(*iv, *iw, EXPD));
						}
	}
}

/**
 * TODO: This is not a good design, using a matrix ??????????
 * @brief determine if (u,v) corresponds to a real transition
 * @param u
 * @param v
 * @param TTD
 * @return bool
 * 			true : if (u, v) is a real edge
 * 			false: otherwise
 */
bool ETTD::is_real_trans(const vertex& u, const vertex& v,
		const adj_list& TTD) {
	auto ifind = TTD.find(u);
	if (ifind == TTD.end())
		return false;
	for (auto iv = ifind->second.begin(); iv != ifind->second.end(); ++iv) {
		if (*iv == v)
			return true;
	}
	return false;
}

/**
 * @brief determine if u.shared = v.shared
 * @param u
 * @param v
 * @return bool
 * 			true : if u.shared = v.shared
 * 			false: otherwise
 */
bool ETTD::is_horizontal(const vertex& u, const vertex& v) {
	return mapping_TS[u].share == mapping_TS[v].share;
}

void ETTD::build_SCC() {
	this->build_SCC(mapping_TS.size(), this->expanded_TTD);
}

void ETTD::build_SCC(const size_V& V, const adj_list& Adj) {
	Graph g(V, Adj);
	g.build_SCC();

	for (auto idlg = Graph::sccs.begin(); idlg !=  Graph::sccs.end(); ++idlg) {
		cout << idlg->first << " size=" << idlg->second.size()<<" ";
		for (auto iv = idlg->second.begin(); iv != idlg->second.end(); ++iv) {
			cout << mapping_TS[*iv] << " ";
		}
		cout << endl;
	}
}

/////////////////////////////////for test//////////////////////////////////////

void ETTD::print_expansion_TTD() {
	cout << "The expanded TTD:\n";
	for (auto isrc = expanded_TTD.begin(); isrc != expanded_TTD.end(); ++isrc)
		for (auto idst = isrc->second.begin(); idst != isrc->second.end();
				++idst)
			cout << mapping_TS[isrc->first] << " -> " << mapping_TS[*idst]
					<< "\n";
	cout << endl;
}

void ETTD::print_transitions() {
	for (const auto& r : transitions)
		cout << r << "\n";
	cout << endl;
}

}
/* namespace SURA */
