/**
 * @name algs.cc
 *
 * @date  : Jul 1, 2015
 * @author: Peizun Liu
 */

#include "algs.hh"

///////////////////////////////////////////////////////////////////////////////

namespace COMPARE {
short compare(clong& x, clong& y) {
	if (x < y)
		return -1;
	if (x > y)
		return +1;
	return 0;
}

/**
 * @brief compare maps
 * @param m1
 * @param m2
 */
short compare_map(const map<ushort, ushort>& m1, const map<ushort, ushort>& m2) {
	auto s1_iter = m1.begin(), s1_end = m1.end();
	auto s2_iter = m2.begin(), s2_end = m2.end();
	while (true) {
		if (s1_iter == s1_end && s2_iter == s2_end) {
			return 0;
		} else if (s1_iter == s1_end) {
			return -1;
		} else if (s2_iter == s2_end) {
			return 1;
		} else if (s1_iter->first < s2_iter->first) {
			return -1;
		} else if (s1_iter->first > s2_iter->first) {
			return 1;
		} else if (s1_iter->first == s2_iter->first) {
			if (s1_iter->second < s2_iter->second) {
				return -1;
			} else if (s1_iter->second > s2_iter->second) {
				return 1;
			}
		}
		s1_iter++, s2_iter++;
	}
	throw ural_rt_err("COMPARE::compare: internal");
}

/**
 * @brief Returns 0 if equal, 1 if x > y, -1 if x < y. (think: sign(x-y))
 * 		Motivation: containers have "bool operator <" defined, but this only returns "<" or ">=".
 * 		When you need to distinguish between all three possibilities, compare is more efficient.
 * 		This function was not called "compare" to avoid confusion with the above compare function
 * @param x
 * @param y
 */
template<class T>
short compare_container(const T& x, const T& y) {
	typename T::const_iterator xi = x.begin(), yi = y.begin(), x_end = x.end(), y_end = y.end();
	while (true) {
		if (xi == x_end && yi == y_end) // x == y
			return 0;
		else if (xi == x_end)                // x < y
			return -1;
		else if (yi == y_end)                // y < x
			return +1;
		else if (*xi < *yi)                  // x < y
			return -1;
		else if (*yi < *xi)                  // y < x
			return +1;
		++xi, ++yi;
	}
	throw ural_rt_err("COMPARE::compare: internal");
}
}

///////////////////////////////////////////////////////////////////////////////

namespace PPRINT {

/**
 * @brief split a string into a vector of strings via a delimiter
 * @param s: object string
 * @param delim: a delimiter
 * @return a vector of string
 */
vector<string> split(const string &s, const char& delim) {
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
vector<string> split(const string &s, char delim, vector<string> &elems) {
	std::stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.emplace_back(item);
	}
	return elems;
}

template<class T>
string width(const T& x, cushort& width) {
	std::ostringstream os;
	os << x;
	string s = os.str();

	vector<string> ss = split(s, '\n');
	if (ss.size() <= 1)
		return s;

	string result = ss[0];
	for (int i = 1; i < ss.size(); i++) {
		result.append("\n").append(string(width, ' ')).append(ss[i]);
	}
	return result;
}
/**
 * @brief Convert object x into a string, padded with fill character as necessary to achieve target width.
 * 		stringstream::operator<< must be defined for object x. If width = 0, just return string version of x
 *
 * @param x
 * @param width
 * @param c
 * @param fill
 * @return
 */
//template<class T>
//string widthify(const T& x, cushort& width, const Alignment& c, const char& fill) {
//	std::ostringstream os;
//	os << x;
//	string s = os.str();
//
//	ushort n = s.size();
//	if (n >= width)
//		return s;
//	ushort addlength = width - n;
//	string result;
//	switch (c) {
//	case LEFTJUST:
//		result = s + string(addlength, fill);
//		break;
//	case RIGHTJUST:
//		result = string(addlength, fill) + s;
//		break;
//	case CENTERED:
//		result = (
//				addlength % 2 == 0 ?
//						string(addlength / 2, fill) + s + string(addlength / 2, fill) :
//						string((addlength - 1) / 2, fill) + s + string((addlength + 1) / 2, fill));
//		break;
//	}
//	return result;
//}
/**
 * @brief Output string s, but with <sep> inserted as tabulator every tab character. Never at left or right
 * 		end of s. Never if either left or right neighbor of tabulator position is ' '
 * @param s
 * @param sep
 * @param tab
 * @return
 */
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

/**
 * @brief Convert object x into a string using sprintf and the format string supplied (which must include the %).
 * 		sprintf must be defined for object x (i.e. mainly basic types, such as numeric ones). If output is expected
 * 		to be very long, provide sufficient length argument
 * @param x
 * @param format
 * @param length
 * @return
 */
template<class T>
string formatString(const T& x, const string& format, cushort& length) {
	char* s = new char[length];
	snprintf(s, length, format.c_str(), x);
	string result = s;
	delete s;
	return result;
}

/**
 * @brief for example:
 * 		given   20, produces "20s",
 * 		given   80, produces "1:20m",
 * 		given 3700, produces "1:01:40h"
 * @param seconds
 * @return
 */
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

/**
 * If firstTime is true, sets it to false, otherwise outputs connective. In any case appends common.
 * Useful to print strings of expressions of the form
 * 			x[0] + x[1] + ... + x[n-1] as follows:
 * bool firstTime = true;
 * for (ushort i = 0; i < n; ++i)
 * cout << PPRINT::firstTimeOrNot(firstTime, " + ", string("x[") + widthify(i) + "]");
 *
 * @param firstTime
 * @param connective
 * @param common
 * @return
 */
string firstTimeOrNot(bool& firstTime, const string& connective, const string& common) {
	string result = (firstTime ? common : connective + common);
	firstTime = false;
	return result;
}
}                     /// end of PPRINT

