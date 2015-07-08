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
	cout << "------------------------" << filename << "\n"; // delete -----------------
	this->parse_input_ttd(filename);

	cout << initl_ts << endl; // delete -----------------
	cout << final_ts << endl; // delete -----------------

	INITL_TS = this->parse_input_tss(initl_ts);
	FINAL_TS = this->parse_input_tss(final_ts);

	return this->reachability_as_logic_decision(original_TTD);
}

/**
 * @brief parse the input TTD
 * @param filename: the name of input ttd file
 */
void Sura::parse_input_ttd(const string& filename) {
	if (filename == "X") { // make random structure
		throw ural_rt_err("Please assign input file");
	} else {
		/// original input file before removing comments
		ifstream org_in(filename.c_str());
		if (!org_in.good())
			throw ural_rt_err("File not found!");

		id_thread_state ts_idx = 0;
		map<Thread_State, id_thread_state> activee_TS;

		Parser::remove_comments(org_in, "/tmp/tmp.ttd.no_comment", "#");
		ifstream new_in("/tmp/tmp.ttd.no_comment"); /// remove comments
		new_in >> Thread_State::S >> Thread_State::L;
		mapping_TS.reserve(Thread_State::S * Thread_State::L);

		Shared_State s1, s2; /// shared states
		Local_State l1, l2;  /// local  states
		id_thread_state src = 0, dst = 0; /// the id of thread states
		string sep;          /// separator
		cout << "-------------11-----------" << filename << "\n"; //TODO delete -----------------
		while (new_in >> s1 >> l1 >> sep >> s2 >> l2) {
			cout << s1 << " " << l1 << " -> " << s2 << " " << l2 << "\n";
			if (sep == "->" || sep == "+>") {
				const Thread_State src_TS(s1, l1);
				/// see if (s1, l1) is already found
				auto ifind = activee_TS.find(src_TS);
				if (ifind != activee_TS.end()) {
					src = ifind->second;
				} else {
					src = ts_idx++;
					mapping_TS.push_back(src_TS);
					activee_TS[src_TS] = src;
				}

				const Thread_State dst_TS(s2, l2);
				/// see if (s2, l2) is already found
				ifind = activee_TS.find(dst_TS);
				if (ifind != activee_TS.end()) {
					dst = ifind->second;
				} else {
					dst = ts_idx++;
					mapping_TS.push_back(dst_TS);
					activee_TS[dst_TS] = dst;
				}

				if (sep == "+>")
					spawntra_TTD[src].push_back(dst);
				original_TTD[src].push_back(dst);

				if (s1 != s2) {
					s_outgoing[s1].insert(src);
					s_incoming[s2].insert(dst);
				}
			} else {
				throw ural_rt_err("illegal transition");
			}
		}
		new_in.close();
		org_in.close();
		mapping_TS.shrink_to_fit();
	}

	cout << mapping_TS.size() << endl; //TODO delete----------
	for (auto i = 0; i < mapping_TS.size(); i++)
		cout << i << " " << mapping_TS[i] << endl;

	cout << "Outgoing:\n";
	for (auto is = s_outgoing.begin(); is != s_outgoing.end(); ++is) {
		cout << "shared state: " << is->first << " ";
		for (auto iv = is->second.begin(); iv != is->second.end(); ++iv) {
			cout << mapping_TS[*iv] << " ";
		}
		cout << endl;
	}

	cout << "Incoming:\n";
	for (auto is = s_incoming.begin(); is != s_incoming.end(); ++is) {
		cout << "shared state: " << is->first << " ";
		for (auto iv = is->second.begin(); iv != is->second.end(); ++iv) {
			cout << mapping_TS[*iv] << " ";
		}
		cout << endl;
	}

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
bool Sura::reachability_as_logic_decision(const adj_list& TTD) {
	ELAPSED_TIME = clock() - ELAPSED_TIME;

	if (INITL_TS == FINAL_TS)
		return true;

	ETTD::instance().print_expansion_TTD();
	ETTD::instance().print_transitions();

	ETTD::instance().build_SCC();
	return false;
}

} /* namespace sura */
