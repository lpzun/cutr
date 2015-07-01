/*
 * utilities.cc
 *
 *  Created on: Jun 23, 2015
 *      Author: lpzun
 */

#include "utilities.hh"
#include "excep.hh"

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
Thread_State Util::create_thread_state_from_str(const string& s_ts, const char& delim) {
	vector<string> vs_ts = Split::split(s_ts, delim);
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
Thread_State Util::create_thread_state_from_gs_str(const string& s_ts, const char& delim) {
	auto vs_ts = Split::split(s_ts, delim);
	if (vs_ts.size() != 2) {
		throw ural_rt_err("The format of global state is wrong.");
	}
	auto vs_locals = Split::split(vs_ts[1], ',');
	TARGET_THR_NUM = vs_locals.size();
	return Thread_State(atol(vs_ts[0].c_str()), atol(vs_locals[0].c_str()));
}
/**
 * @brief print all of the transitions in the thread-state transition diagram
 * @param adjacency_list
 * @param out
 */
void Util::print_adj_list(const map<Thread_State, set<Thread_State> >& adj_list, ostream& out) {
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
void Util::print_adj_list(const map<Thread_State, list<Thread_State> >& adj_list, ostream& out) {
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
 * @brief split a string into a vector of strings via a delimiter
 * @param s: object string
 * @param delim: a delimiter
 * @return a vector of string
 */
vector<string> Split::split(const string &s, const char& delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

/**
 * @brief split a string into a vector of strings via a delimiter
 * @param s: object string
 * @param delim: a delimiter
 * @param elems
 * @return a vector of string
 */
vector<string> Split::split(const string &s, char delim, vector<string> &elems) {
	std::stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

//namespace PARSER {

/**
 * @brief remove the comments of .ttd files
 * @param in
 * @param filename
 * @param comment
 */
void Parser::remove_comments(istream& in, const string& filename, const string& comment) {
	ofstream out(filename.c_str());
	remove_comments(in, out, comment);
}

/**
 * @brief remove the comments of .ttd files
 * @param in
 * @param out
 * @param comment
 */
void Parser::remove_comments(const string& in, string& out, const string& comment) {
	std::istringstream instream(in);
	std::ostringstream outstream;
	remove_comments(instream, outstream, comment);
	out = outstream.str();
}

void Parser::remove_comments(istream& in, ostream& out, const string& comment) {
	string line;
	while (this->getline(in, line = "")) {
		const size_t comment_start = line.find(comment);
		out << (comment_start == string::npos ? line : line.substr(0, comment_start)) << endl;
	}
}

bool Parser::getline(istream& in, string& line, const char& eol) {
	char c = 0;
	while (in.get(c) ? c != eol : false)
		line += c;
	return c != 0;
}

/// end of PARSER

namespace PPRINT {
string tabularize(const string& s, const string& sep, const unsigned short& tab) {
	string result;
	ushort n = s.size();
	for (int i = 0; i < n; ++i) {
		if (i > 0 && (n - i) % tab == 0)
			result += (s[i - 1] == ' ' || s[i] == ' ' ? " " : sep);
		result += s[i];
	}
	return result;
}

string hourize(culong& seconds) {
	if (seconds < 60)
		return widthify(seconds) + "s";
	else if (seconds < 3600) { // 60 <= seconds < 3600
		ushort m = seconds / 60;
		ushort s = seconds % 60;
		return widthify(m) + ":" + widthify(s, 2, RIGHTJUST, '0') + "m";
	} else {                     // 3600 <= seconds < infty
		ushort h = seconds / 3600;
		ushort s = seconds % 3600;
		ushort m = s / 60;
		s = s % 60;
		return widthify(h) + ":" + widthify(m, 2, RIGHTJUST, '0') + ":" + widthify(s, 2, RIGHTJUST, '0') + "h";
	}
}

string firstTimeOrNot(bool& firstTime, const string& connective, const string& common) {
	string result = (firstTime ? common : connective + common);
	firstTime = false;
	return result;
}
} /// end of PPRINT

} /* namespace SURA */
