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
bool Sura::symbolic_reachability_analysis(const string& filename,
		const string& initl_ts, const string& final_ts) {
	INITL_TS = this->parse_input_tss(initl_ts);
	FINAL_TS = this->parse_input_tss(final_ts);
	return this->reachability_as_logic_decision(original_TTD,
			parse_input_ttd(filename));
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
		mapping_TS.reserve(Thread_State::S * Thread_State::L);

		id_thread_state idx = 0;

		/// setting the initial vertex
		mapping_TS.emplace_back(INITL_TS);
		activee_TS.emplace(INITL_TS, idx);
		INITL_V = idx++;
		cout << idx << "################################\n";

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
			cout << s1 << " " << l1 << " -> " << s2 << " " << l2 << "\n"; //TODO delete -----------------
			if (sep == "->" || sep == "+>") {
				const Thread_State src_TS(s1, l1);
				/// see if (s1, l1) is already found
				auto ifind = activee_TS.find(src_TS);
				if (ifind != activee_TS.end()) {
					src = ifind->second;
				} else {
					src = idx++;
					mapping_TS.emplace_back(src_TS);
					activee_TS.emplace(src_TS, src);
				}

				const Thread_State dst_TS(s2, l2);

				/// see if (s2, l2) is already found
				ifind = activee_TS.find(dst_TS);
				if (ifind != activee_TS.end()) {
					dst = ifind->second;
				} else {
					dst = idx++;
					mapping_TS.emplace_back(dst_TS);
					activee_TS.emplace(dst_TS, dst);
				}

				if (sep == "+>")
					spawntra_TTD[src].emplace_back(dst);
				original_TTD[src].emplace_back(dst);

				if (s1 != s2) {
					s_in_out[s1].second.emplace(src);
					s_in_out[s2].first.emplace(dst);
				}
			} else {
				throw ural_rt_err("illegal transition");
			}
		}
		new_in.close();
		org_in.close();
		mapping_TS.shrink_to_fit();

		cout << idx << "################################\n";

		/// setting the final vertex
		auto ifind = activee_TS.find(FINAL_TS);
		if (ifind != activee_TS.end()) {
			FINAL_V = ifind->second;
		} else {
			mapping_TS.emplace_back(FINAL_TS);
			activee_TS.emplace(FINAL_TS, idx);
			FINAL_V = idx;
		}

		s_in_out[FINAL_TS.get_share()].second.emplace(FINAL_V);

		for (auto i = 0; i < mapping_TS.size(); ++i) {
			cout << i << ": " << mapping_TS[i] << "\n";
		}
		cout << endl;

#ifndef NDEBUG
		cout << INITL_TS << " $$$$$$$$$ " << FINAL_TS << endl;
		cout << INITL_V << " $$$$$$$$$ " << FINAL_V << endl;

		cout << "Incoming:\n";
		for (auto is = 0; is < s_in_out.size(); ++is) {
			cout << "shared state: " << is << " ";
			for (auto iv = s_in_out[is].first.begin();
					iv != s_in_out[is].first.end(); ++iv) {
				cout << mapping_TS[*iv] << " ";
			}
			cout << endl;
		}

		cout << "Outgoing:\n";
		for (auto is = 0; is < s_in_out.size(); ++is) {
			cout << "shared state: " << is << " ";
			for (auto iv = s_in_out[is].second.begin();
					iv != s_in_out[is].second.end(); ++iv) {
				cout << mapping_TS[*iv] << " ";
			}
			cout << endl;
		}

	}

	cout << mapping_TS.size() << endl;
	for (auto i = 0; i < mapping_TS.size(); i++)
		cout << i << " " << mapping_TS[i] << endl;
#endif

	if (OPT_PRINT_ADJ || OPT_PRINT_ALL) {
		cout << "The original TTD:" << endl;
		for (auto isrc = original_TTD.begin(); isrc != original_TTD.end();
				++isrc) {
			for (auto idst = isrc->second.begin(); idst != isrc->second.end();
					++idst) {
				cout << mapping_TS[isrc->first] << " -> " << mapping_TS[*idst]
						<< " ";
				cout << isrc->first << " -> " << *idst << "\n";
			}
		}
		cout << endl;
	}
	return s_in_out;
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
bool Sura::reachability_as_logic_decision(const adj_list& TTD,
		const vector<inout>& s_in_out) {

	ELAPSED_TIME = clock() - ELAPSED_TIME;

	if (INITL_TS == FINAL_TS)
		return true;

	ETTD ettd(TTD, s_in_out);
	ettd.print_expanded_TTD();
	ettd.print_transitions();

	shared_ptr<GSCC> p_gscc = std::make_shared<GSCC>(ettd.get_V(),
			ettd.get_expanded_TTD());

	// testing delete------------------------------------------------------
	for (auto i = p_gscc->get_sccs().begin(); i != p_gscc->get_sccs().end();
			++i) {
		if (*i != nullptr)
			cout << **i << endl;
	}
	// testing delete------------------------------------------------------

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
	const auto& list_P = p_gscc->find_all_paths();
	auto size_P = list_P.size();
	FWS fws(size_P, p_gscc);
	return fws.fws_as_logic_decision(list_P);
}

} /* namespace sura */
