/**
 * @name algs.hh
 *
 * @date  : Jul 1, 2015
 * @author: Peizun Liu
 */

#ifndef ALGS_HH_
#define ALGS_HH_

#include "heads.hh"

/// namespace COMPARE
namespace COMPARE {
short compare(clong& x, clong& y);

short compare_map(const map<ushort, ushort>& m1, const map<ushort, ushort>& m2);
template<class T>
short compare_container(const T& x, const T& y);
}

namespace PPRINT {
vector<string> split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, const char& delim);

typedef enum {
	LEFTJUST, RIGHTJUST, CENTERED
} Alignment;

template<class T>
string width(const T& x, cushort& width = 0);

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

template<class T>
string formatString(const T& x, const string& format, cushort& length = 10);

string tabularize(const string& s, const string& sep = ",", cushort& tab = 3);
string hourize(culong& seconds);
string firstTimeOrNot(bool& firstTime, const string& connective, const string& common);

inline string plural(culong& n, const string& name, const string& suffix = "s") {
	return widthify(n) + " " + name + (n == 1 ? "" : suffix);
}
}

#endif /* ALGS_HH_ */
