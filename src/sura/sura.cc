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
 * @param infile  : the input ttd file
 * @param initl_ts: initial thread state
 * @param final_ts: final   thread state
 * @return
 */
bool Sura::symbolic_reachability_analysis(const string& infile, const string& initl_ts, const string& final_ts) {

	return false;
}

void Sura::parse_input_ttd(const string& infile) {
	if (infile == "X") { // make random structure
		throw ural_rt_err("Please assign input file");
	} else {
		/// original input file before removing comments
		ifstream org_in(infile.c_str());
		if (!org_in.good())
			throw ural_rt_err("File not found!");

		thread_state_id ts_idx = 0;
		map<Thread_State, thread_state_id> relevent_TS;

		Parser::remove_comments(org_in, "/tmp/tmp.ttd.no_comment", "#");
		ifstream new_in("/tmp/tmp.ttd.no_comment"); /// new input file after removing comments
		new_in >> Thread_State::S >> Thread_State::L;
		ushort s1, l1, s2, l2;
		thread_state_id src, dst;
		string sep;
		while (new_in) {
			new_in >> s1 >> l1 >> sep >> s2 >> l2;
			if (sep == "->" || sep == "+>") {

				auto ifind = relevent_TS.find(Thread_State(s1, l1));
				if (ifind != relevent_TS.end()) {
					src = ifind->second;
				} else {
					src = ts_idx++;
				}

				ifind = relevent_TS.find(Thread_State(s2, l2));
				if (ifind != relevent_TS.end()) {
					dst = ifind->second;
				} else {
					dst = ts_idx++;
				}

				if (sep == "+>")
					spawntra_TTD[src].push_back(dst);
				original_TTD[src].push_back(dst);
			} else {
				throw ural_rt_err("illegal transition");
			}
		}
		original_TTD[Thread_State(s1, l1)].pop_back();
		new_in.close();
		org_in.close();
	}

	if (OPT_PRINT_ADJ || OPT_PRINT_ALL) {
		cout << "The original TTD:" << endl;
		Util::print_adj_list(original_TTD); // print out the adjency list
	}
}

void Sura::parse_input_tss(const string& ts) {

}

} /* namespace sura */
