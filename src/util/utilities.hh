/*
 * utilities.hh
 *
 *  Created on: Jun 23, 2015
 *      Author: lpzun
 */

#ifndef UTILITIES_HH_
#define UTILITIES_HH_

#include "heads.hh"
#include "state.hh"
#include "refs.hh"

namespace sura {

class Utilities {
public:
	Utilities();
	virtual ~Utilities();
	static Thread_State create_thread_state_from_str(const string& s_ts, const char& delim = '|');
	static Thread_State create_thread_state_from_gs_str(const string& s_ts, const char& delim = '|');

	static void print_adj_list(const map<Thread_State, set<Thread_State> >& adj_list, ostream& out = cout);
	static void print_adj_list(const map<Thread_State, list<Thread_State> >& adj_list, ostream& out = cout);

//	static void output_ettd_to_dot_file(const string& filename, const list<Transition<Thread_State>>& transitions);
};

class Split {
private:
	static vector<string> split(const string &s, char delim, vector<string> &elems);
public:
	Split() = default;
	virtual ~Split() {
	}
	static vector<string> split(const string &s, const char& delim);
};

namespace PPRINT {
typedef enum {
	LEFTJUST, RIGHTJUST, CENTERED
} Alignment;

template<class T>
string width(const T& x, cushort& width = 0) {
	std::ostringstream os;
	os << x;
	string s = os.str();

	vector<string> ss = Split::split(s, '\n');
	if (ss.size() <= 1)
		return s;

	string result = ss[0];
	for (int i = 1; i < ss.size(); i++) {
		result.append("\n").append(string(width, ' ')).append(ss[i]);
	}
	return result;
}

// Convert object x into a string, padded with fill character as necessary to achieve target width.
// stringstream::operator<< must be defined for object x.
// If width = 0, just return string version of x
template<class T>
string widthify(const T& x, cushort& width = 0, const Alignment& c = CENTERED, const char& fill = ' ') {
	std::ostringstream os;
	os << x;
	string s = os.str();

	ushort n = s.size();
	if (n >= width)
		return s;
	ushort addlength = width - n;
	string result;
	switch (c) {
	case LEFTJUST:
		result = s + string(addlength, fill);
		break;
	case RIGHTJUST:
		result = string(addlength, fill) + s;
		break;
	case CENTERED:
		result = (
				addlength % 2 == 0 ?
						string(addlength / 2, fill) + s + string(addlength / 2, fill) :
						string((addlength - 1) / 2, fill) + s + string((addlength + 1) / 2, fill));
		break;
	}
	return result;
}

// Output string s, but with <sep> inserted as tabulator every tab character. Never at left or right end of s.
// Never if either left or right neighbor of tabulator position is ' '
string tabularize(const string& s, const string& sep = ",", cushort& tab = 3);

// Convert object x into a string using sprintf and the format string supplied (which must include the %).
// sprintf must be defined for object x (i.e. mainly basic types, such as numeric ones).
// If output is expected to be very long, provide sufficient length argument
template<class T>
string formatString(const T& x, const string& format, cushort& length = 10) {
	char* s = new char[length];
	snprintf(s, length, format.c_str(), x);
	string result = s;
	delete s;
	return result;
}

string hourize(culong& seconds); // given 20, produces "20s", given 80, produces "1:20m", given 3700, produces "1:01:40h"
inline string varname(const string& prefix = "") {
	static ushort varname_counter = 0;
	return prefix + "_" + widthify(varname_counter++);
}
inline string plural(culong& n, const string& name, const string& suffix = "s") {
	return widthify(n) + " " + name + (n == 1 ? "" : suffix);
}

// If firstTime is true, sets it to false, otherwise outputs connective. In any case appends common.
// Useful to print strings of expressions of the form
// x[0] + x[1] + ... + x[n-1] as follows:
// bool firstTime = true;
// for (ushort i = 0; i < n; ++i)
//   cout << PPRINT::firstTimeOrNot(firstTime, " + ", string("x[") + widthify(i) + "]");
string firstTimeOrNot(bool& firstTime, const string& connective, const string& common);
}
} /* namespace SURA */

#endif /* UTILITIES_HH_ */
