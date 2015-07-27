/**
 * @name  : sura.cc
 *
 * @date  : Jun 25, 2015
 * @author: Peizun Liu
 */

#include "sura.hh"

namespace sura {

Sura::Sura() {
	// TODO Auto-generated constructor stub

}

Sura::~Sura() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief symbolic unbounded-thread reachability analysis
 * @param filename: the name of input ttd file
 * @param initl_ts: initial thread state
 * @param final_ts: final   thread state
 * @return
 */
bool Sura::symbolic_reachability_analysis(const string& filename, const string& initl_ts, const string& final_ts) {
	Refs::INITL_TS = this->parse_input_tss(initl_ts);
	Refs::FINAL_TS = this->parse_input_tss(final_ts);

	/// if initial thread state is equal to final thread state,
	/// then return true
	if (Refs::INITL_TS == Refs::FINAL_TS)
		return true;
//	return this->fws(Refs::INITL_N, Refs::SPAWN_Z);
	return this->reachability_as_logic_decision(Refs::original_TTD, parse_input_ttd(filename));
}

/**
 * @brief parse the input TTD
 * @param filename: the name of input .ttd file
 */
vector<inout> Sura::parse_input_ttd(const string& filename) {
	vector<inout> s_in_out;
	if (filename == "X") { // make random structure
		throw ural_rt_err("Please assign the input file!");
	} else {
		/// original input file before removing comments
		ifstream org_in(filename.c_str());
		if (!org_in.good())
			throw ural_rt_err("Input file does not find!");

		Parser::remove_comments(org_in, "/tmp/tmp.ttd.no_comment", "#");
		ifstream new_in("/tmp/tmp.ttd.no_comment"); /// remove comments

		new_in >> Thread_State::S >> Thread_State::L;
		Refs::mapping_TS.reserve(Thread_State::S * Thread_State::L);

		id_thread_state id_TS = 0; /// the id of thread state

		/// setting the initial vertex
		Refs::mapping_TS.emplace_back(Refs::INITL_TS);
		Refs::activee_TS.emplace(Refs::INITL_TS, id_TS);
		INITL_V = id_TS++;

		/// store all outgoing vertices from same shared state
		/// key is the shared state,
		///        first  value is the set of incoming vertices
		///        second value is the set of outgoing vertices
		s_in_out = vector<inout>(Thread_State::S);

		Shared_State s1, s2;              /// shared states
		Local_State l1, l2;               /// local  states
		id_thread_state src = 0, dst = 0; /// the id of thread state
		string sep;                       /// separator
		while (new_in >> s1 >> l1 >> sep >> s2 >> l2) {
			DBG_STD(cout << s1 << " " << l1 << " -> " << s2 << " " << l2 << "\n")
			if (l1 == l2) /// remove self loops and vertical transitions
				continue;
			if (sep == "->" || sep == "+>") {
				const Thread_State src_TS(s1, l1);
				/// see if (s1, l1) is already found
				auto ifind = Refs::activee_TS.find(src_TS);
				if (ifind != Refs::activee_TS.end()) {
					src = ifind->second;
				} else {
					src = id_TS++;
					Refs::mapping_TS.emplace_back(src_TS);
					Refs::activee_TS.emplace(src_TS, src);
				}

				const Thread_State dst_TS(s2, l2);
				/// see if (s2, l2) is already found
				ifind = Refs::activee_TS.find(dst_TS);
				if (ifind != Refs::activee_TS.end()) {
					dst = ifind->second;
				} else {
					dst = id_TS++;
					Refs::mapping_TS.emplace_back(dst_TS);
					Refs::activee_TS.emplace(dst_TS, dst);
				}

				if (sep == "+>")
					Refs::spawntra_TTD[src].emplace_back(dst);
				Refs::original_TTD[src].emplace_back(dst);
				if (true || s1 != s2) {
					s_in_out[s1].second.emplace(src);
					s_in_out[s2].first.emplace(dst);
				}
			} else {
				throw ural_rt_err("illegal transition");
			}
		}
		new_in.close();
		org_in.close();
		Refs::mapping_TS.shrink_to_fit();

		/// setting the final vertex
		auto ifind = Refs::activee_TS.find(Refs::FINAL_TS);
		if (ifind != Refs::activee_TS.end()) {
			FINAL_V = ifind->second;
		} else {
			Refs::mapping_TS.emplace_back(Refs::FINAL_TS);
			Refs::activee_TS.emplace(Refs::FINAL_TS, id_TS);
			FINAL_V = id_TS;
		}

		s_in_out[Refs::FINAL_TS.get_share()].second.emplace(FINAL_V);

#ifndef NDEBUG
		cout << __func__ << "\n";
		cout << "Initial Thread State " << Refs::INITL_TS << "\t";
		cout << "Final Thread State " << Refs::FINAL_TS << "\n";
		cout << "Initial vertex " << INITL_V << "\t";
		cout << "Final vertex " << FINAL_V << "\n";

		cout << "Incoming:\n";
		for (size_t is = 0; is < s_in_out.size(); ++is) {
			cout << "shared state: " << is << " ";
			for (auto iv = s_in_out[is].first.begin(); iv != s_in_out[is].first.end(); ++iv)
			cout << Refs::mapping_TS[*iv] << " ";
			cout << "\n";
		}

		cout << "Outgoing:\n";
		for (size_t is = 0; is < s_in_out.size(); ++is) {
			cout << "shared state: " << is << " ";
			for (auto iv = s_in_out[is].second.begin(); iv != s_in_out[is].second.end(); ++iv)
			cout << Refs::mapping_TS[*iv] << " ";
			cout << "\n";
		}

		cout << Refs::mapping_TS.size() << "\n";
		for (size_t i = 0; i < Refs::mapping_TS.size(); i++)
		cout << i << " " << Refs::mapping_TS[i] << "\n";
		cout << endl;
#endif

		if (Refs::OPT_PRINT_ADJ || Refs::OPT_PRINT_ALL) {
			cout << "The original TTD:" << endl;
			for (auto isrc = Refs::original_TTD.begin(); isrc != Refs::original_TTD.end(); ++isrc) {
				for (auto idst = isrc->second.begin(); idst != isrc->second.end(); ++idst) {
					cout << Refs::mapping_TS[isrc->first] << " -> " << Refs::mapping_TS[*idst] << " ";
					cout << isrc->first << " -> " << *idst << "\n";
				}
			}
			cout << endl;
		}
		return s_in_out;
	}
}

/**
 * @brief parse the input initial and final thread state
 * @param str_ts: the thread state represented by string
 */
Thread_State Sura::parse_input_tss(const string& str_ts) {
	Thread_State ts;
	if (str_ts.find('|') != std::string::npos) {
		ts = Util::create_thread_state_from_gs_str(str_ts);
	} else { /// str_ts is store in a file
		ifstream in(str_ts.c_str());
		if (in.good()) {
			string s_ts;
			std::getline(in, s_ts);
			ts = Util::create_thread_state_from_gs_str(s_ts);
			in.close();
		} else {
			throw ural_rt_err("read_in_thread_state: unknown input file");
		}
	}
	return ts;
}

/**
 * @brief reduce a reachability problem as a logic decision problem
 * @param TTD
 * @return bool
 */
bool Sura::reachability_as_logic_decision(const adj_list& TTD, const vector<inout>& s_in_out) {
	Refs::ELAPSED_TIME = clock() - Refs::ELAPSED_TIME;

	Graph g(Refs::mapping_TS.size(), TTD);
	if (g.is_reachable(INITL_V, FINAL_V))
		return true;
	ETTD ettd(TTD, s_in_out);
#ifndef NDEBUG
	cout << "print out ETTD and expanded transitions:\n";
	ettd.print_expanded_TTD();
	ettd.print_transitions();
#endif
	shared_ptr<GSCC> p_gscc = std::make_shared<GSCC>(ettd.get_V(), ettd.get_expanded_TTD());
#ifndef NDEBUG
	cout << "print out all SCCs:\n";
	for (auto i = p_gscc->get_sccs().begin(); i != p_gscc->get_sccs().end(); ++i) {
		if (*i != nullptr)
		cout << **i << "\n";
	}
#endif
	Refs::ELAPSED_TIME = clock() - Refs::ELAPSED_TIME;
	cout << "Build SCC quotient paths time: " << (double(Refs::ELAPSED_TIME)) / CLOCKS_PER_SEC << endl;
	return this->path_wise_analysis(p_gscc);
}

/**
 * @brief pathwise analysis
 * @param paths
 * @return bool
 * 		true : if the final thread state is reachable
 * 		false: otherwise
 */
bool Sura::path_wise_analysis(const shared_ptr<GSCC>& p_gscc) {
	const auto& paths = p_gscc->find_all_paths();
	auto size_P = paths.size();
	if (size_P < 1) {
		cout << "There is no path between " << Refs::INITL_TS << " and " << Refs::FINAL_TS << endl;
		return false;
	}
	size_P = 0;
	for (auto ipath = paths.begin(); ipath != paths.end(); ++ipath) {
		size_t num = 1;
		for (auto iscc = ipath->begin(); std::next(iscc) != ipath->end(); ++iscc) {
			if (p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(iscc))] != nullptr)
				num *= (p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(iscc))])->size();
		}
		size_P += num;
	}
	cout << "The number of paths is: " << size_P << endl;
	FWS fws(size_P, p_gscc);
	return fws.fws_as_logic_decision(paths);
}

} /* namespace sura */
