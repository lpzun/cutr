/**
 * @name utilities.hh
 *
 * @date  : Jun 23, 2015
 * @author: Peizun Liu
 */

#ifndef UTILITIES_HH_
#define UTILITIES_HH_

#include "heads.hh"
#include "refs.hh"

namespace sura {

class Util {
public:
	Util();
	virtual ~Util();
	static Thread_State create_thread_state_from_str(const string& s_ts, const char& delim = '|');
	static Thread_State create_thread_state_from_gs_str(const string& s_ts, const char& delim = '|');

	static void print_adj_list(const map<Thread_State, set<Thread_State> >& adj_list, ostream& out = cout);
	static void print_adj_list(const map<Thread_State, deque<Thread_State> >& adj_list, ostream& out = cout);

//	static void output_ettd_to_dot_file(const string& filename, const list<Transition<Thread_State>>& transitions);
};

class Parser {
public:
	static void remove_comments(istream& in, const string& filename, const string& comment);
	static void remove_comments(const string& in, string& out, const string& comment);

private:
	static void remove_comments(istream& in, ostream& out, const string& comment);
	static bool getline(istream& in, string& line, const char& eol = '\n');
};

} /* namespace SURA */

#endif /* UTILITIES_HH_ */
