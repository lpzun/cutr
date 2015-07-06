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
	cout << "------------------------" << filename << "\n"; // delete -----------------
	this->parse_input_ttd(filename);
	return false;
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
		map<Thread_State, id_thread_state> active_TS;

		Parser::remove_comments(org_in, "/tmp/tmp.ttd.no_comment", "#");
		ifstream new_in("/tmp/tmp.ttd.no_comment"); /// new input file after removing comments
		new_in >> Thread_State::S >> Thread_State::L;
		mapping_TS.reserve(Thread_State::S * Thread_State::L);

		Shared_State s1, s2; /// shared states
		Local_State l1, l2;  /// local  states
		id_thread_state src = 0, dst = 0; /// the id of thread states
		string sep;          /// separator
		cout << "-------------11-----------" << filename << "\n"; // delete -----------------
		while (new_in >> s1 >> l1 >> sep >> s2 >> l2) {
			cout << s1 << " " << l1 << " -> " << s2 << " " << l2 << "\n";
			if (sep == "->" || sep == "+>") {
				const Thread_State src_TS(s1, l1);
				/// see if (s1, l1) is already found
				auto ifind = active_TS.find(src_TS);
				if (ifind != active_TS.end()) {
					src = ifind->second;
				} else {
					src = ts_idx++;
					mapping_TS.push_back(src_TS);
					active_TS[src_TS] = src;
				}

				const Thread_State dst_TS(s2, l2);
				/// see if (s2, l2) is already found
				ifind = active_TS.find(dst_TS);
				if (ifind != active_TS.end()) {
					dst = ifind->second;
				} else {
					dst = ts_idx++;
					mapping_TS.push_back(dst_TS);
					active_TS[dst_TS] = dst;
				}

				if (sep == "+>")
					spawntra_TTD[src].push_back(dst);
				original_TTD[src].push_back(dst);
			} else {
				throw ural_rt_err("illegal transition");
			}
		}
		new_in.close();
		org_in.close();
		mapping_TS.shrink_to_fit();
	}

	for (auto i = 0; i < mapping_TS.size(); i++)
		cout << i << " " << mapping_TS[i] << endl;

	if (OPT_PRINT_ADJ || OPT_PRINT_ALL) {
		cout << "The original TTD:" << endl;
		for (auto isrc = original_TTD.begin(); isrc != original_TTD.end(); ++isrc) {
			for (auto idst = isrc->second.begin(); idst != isrc->second.end(); ++idst) {
				cout << mapping_TS[isrc->first] << " -> " << mapping_TS[*idst] << " ";
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
void Sura::parse_input_tss(const string& str_ts) {

}

} /* namespace sura */
