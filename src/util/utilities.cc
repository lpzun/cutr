/**
 * @name utilities.hh
 *
 * @date  : Jun 23, 2015
 * @author: Peizun Liu
 */

#include "utilities.hh"

namespace sura {

Util::Util() {
	// TODO Auto-generated constructor stub

}

Util::~Util() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param s_ts
 * @param delim
 * @return
 */
Thread_State Util::create_thread_state_from_str(const string& s_ts,
		const char& delim) {
	vector<string> vs_ts = PPRINT::split(s_ts, delim);
	if (vs_ts.size() != 2) {
		throw("The format of thread state is wrong.");
	}
	return Thread_State(atol(vs_ts[0].c_str()), atol(vs_ts[1].c_str()));
}

/**
 *
 * @param s_ts
 * @param delim
 * @return
 */
Thread_State Util::create_thread_state_from_gs_str(const string& s_ts,
		const char& delim) {
	auto vs_ts = PPRINT::split(s_ts, delim);
	if (vs_ts.size() != 2) {
		throw ural_rt_err("The format of global state is wrong.");
	}
	auto vs_locals = PPRINT::split(vs_ts[1], ',');
	Refs::TARGET_THR_NUM = vs_locals.size();
	return Thread_State(atol(vs_ts[0].c_str()), atol(vs_locals[0].c_str()));
}
/**
 * @brief print all of the transitions in the thread-state transition diagram
 * @param adjacency_list
 * @param out
 */
void Util::print_adj_list(const map<Thread_State, set<Thread_State> >& adj_list,
		ostream& out) {
	out << Thread_State::L << " " << Thread_State::S << endl;
	for (auto iu = adj_list.begin(); iu != adj_list.end(); ++iu)
		for (auto iv = iu->second.begin(); iv != iu->second.end(); ++iv)
			out << iu->first << " -> " << (*iv) << endl;
}

/**
 * @brief print all of the transitions in the thread-state transition diagram
 * @param adjacency_list
 * @param out
 */
void Util::print_adj_list(
		const map<Thread_State, deque<Thread_State> >& adj_list, ostream& out) {
	out << Thread_State::L << " " << Thread_State::S << endl;
	for (auto iu = adj_list.begin(); iu != adj_list.end(); ++iu)
		for (auto iv = iu->second.begin(); iv != iu->second.end(); ++iv)
			out << iu->first << " -> " << (*iv) << endl;
}

/**
 * @brief to generate the .dot file for TTD
 * @param filename
 * @param transitions
 */
//void Utilities::output_ettd_to_dot_file(const string& filename, const list<Transition<Thread_State>>& transitions) {
//	ofstream fout;
//	fout.open(filename.c_str());
//	if (!fout.is_open()) {
//		cerr << " create file " << filename << " failed!" << endl;
//	} else {
//		fout << "// This file is generated from Press, which aims to visualize the Expanded TTD\n";
//		fout << "// NOTE: convert .dot to .ps using the following command:\n";
//		fout << "//\t\t neato -Tps -n2 -s1 <.dot file> -o <.ps file>\n";
//		fout << "// You can also convert to other format, say fig, with corresponding commands\n";
//		fout << "\n";
//
//		fout << "digraph ttd {\n";
//		fout << "\t pagedir=BL; nodesep =.5; rankdir=BT; overlap=false; splines = spline;\n";
//		fout << "\t node[shape=plaintext];\n";
//		fout << "\t edge [arrowhead=vee, arrowsize=0.5];\";
//
//		float x = 0.0, y = 0.0;
//		for (ushort s = 0; s < Thread_State::S; s++) {
//			for (ushort l = 0; l < Thread_State::L; l++) {
//				fout << "\t \"(" << s << "," << l << ")\"" << "[pos=\"" << x << "," << y << "\"];\n";
//				x += 80.0;
//			}
//			fout << "\n";
//			y += 50.0;
//			x = 0.0;
//		}
//
//		fout << "\n";
//		for (auto itran = transitions.begin(), end = transitions.end(); itran != end; itran++) {
//			fout << "\t \"(" << itran->src.share << "," << itran->src.local << ")\" -> \"(" << itran->dst.share << ","
//					<< itran->dst.local << ")\"";
//			if (itran->type == EXPD) {
//				fout << "[style=dotted, color=red]";
//			}
//			fout << ";\n";
//		}
//
//		fout << "}" << endl;
//		fout.flush();
//		fout.close();
//	}
//}
/**
 * @brief remove the comments of .ttd files
 * @param in
 * @param filename
 * @param comment
 */
void Parser::remove_comments(istream& in, const string& filename,
		const string& comment) {
	ofstream out(filename.c_str());
	remove_comments(in, out, comment);
}

/**
 * @brief remove the comments of .ttd files
 * @param in
 * @param out
 * @param comment
 */
void Parser::remove_comments(const string& in, string& out,
		const string& comment) {
	std::istringstream instream(in);
	std::ostringstream outstream;
	remove_comments(instream, outstream, comment);
	out = outstream.str();
}

void Parser::remove_comments(istream& in, ostream& out, const string& comment) {
	string line;
	while (getline(in, line = "")) {
		const size_t comment_start = line.find(comment);
		out
				<< (comment_start == string::npos ?
						line : line.substr(0, comment_start)) << endl;
	}
}

bool Parser::getline(istream& in, string& line, const char& eol) {
	char c = 0;
	while (in.get(c) ? c != eol : false)
		line += c;
	return c != 0;
}

/// end of PARSER
} /* namespace SURA */
