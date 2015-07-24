/******************************************************************************
 *  @name cmd.cc
 *
 *  @date  : Nov 18, 2014
 *  @author: Peizun Liu
 *****************************************************************************/

#ifndef CMD_HH_
#define CMD_HH_
#include <iostream>
#include <ostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "algs.hh"

using std::ostream;
using std::string;
using std::map;
using std::list;
using std::vector;
using std::cout;
using std::endl;

extern string v_info;

class Arguments {
public:
	inline Arguments(const short& type, const string& short_name,
			const string& long_name, const string& meaning) :
			type(type), short_name(short_name), long_name(long_name), meaning(
					meaning) {
	}

	inline ~Arguments() {
	}

	inline const short& get_type() const {
		return type;
	}

	inline const string& get_long_name() const {
		return long_name;
	}

	inline const string& get_meaning() const {
		return meaning;
	}

	inline const string& get_short_name() const {
		return short_name;
	}

protected:
	const short type;
	const string short_name; /// the argument's long name
	const string long_name;  /// the argument's short name
	const string meaning;    /// the meaning of the argument
};

inline bool operator==(const Arguments& arg1, const Arguments& arg2) {
	return (arg1.get_long_name() == arg2.get_long_name());
}

inline bool operator<(const Arguments& arg1, const Arguments& arg2) {
	return (arg1.get_long_name() < arg2.get_long_name());
}

class Options: public Arguments {
public:
	inline Options(const short& type, const string& short_name,
			const string& long_name, const string& meaning,
			const string& default_value) :
			Arguments(type, short_name, long_name, meaning), value(
					default_value) {
	}

	inline Options(const short& type = 0, const string& long_name = "") :
			Arguments(type, "", long_name, "") {
	}

	inline ~Options() {
	}

	inline const string& get_value() const {
		return value;
	}

	inline void set_value(const string& value) {
		this->value = value;
	}

private:
	string value; /// the value of this argument
};

inline bool operator==(const Options& arg1, const Options& arg2) {
	return (arg1.get_long_name() == arg2.get_long_name());
}

inline bool operator<(const Options& arg1, const Options& arg2) {
	return (arg1.get_long_name() < arg2.get_long_name());
}

class Switch: public Arguments {
public:
	inline Switch(const short& type, const string& short_name,
			const string& long_name, const string& meaning) :
			Arguments(type, short_name, long_name, meaning), value(false) {
	}

	inline Switch(const short& type = 0, const string& long_name = "") :
			Arguments(type, "", long_name, ""), value(false) {
	}

	inline ~Switch() {

	}

	bool is_value() const {
		return value;
	}

	void set_value(bool value) {
		this->value = value;
	}

private:
	bool value; /// the value of this switch
};

inline bool operator==(const Switch& arg1, const Switch& arg2) {
	return (arg1.get_long_name() == arg2.get_long_name());
}

inline bool operator<(const Switch& arg1, const Switch& arg2) {
	return (arg1.get_long_name() < arg2.get_long_name());
}

#define DEFAULT_OPTS 0
#define PROB_INST_OPTS 1
#define EXP_MODE_OPTS 2
#define SMT_SOLVER_OPTS 3
#define OTHER_OPTS 3

#define OPTS_TYPES 4

#define SHORT_HELP_OPT "-h"
#define LONG_HELP_OPT "--help"
#define SHORT_VERSION_OPT "-v"
#define LONG_VERSION_OPT "--version"

const string VERSION = "v1.0";

const string OPT_MODE_FWS = "F";
const string OPT_MODE_LDP = "S";
const string OPT_MODE_CON = "L";

class Cmd_Line {
public:
	Cmd_Line();
	virtual ~Cmd_Line();

	struct Help {
	};

	void add_option(const short& type, const string& short_name,
			const string& long_name, const string& meaning,
			const string& default_value);
	void add_switch(const short& type, const string& short_name,
			const string& long_name, const string& meaning);

	void get_command_line(const int argc, const char* const * const argv); // args supplied as by main
	void get_command_line(const string& prog, const vector<string>& args);

	bool arg_bool(const short& type, const string& arg);
	string arg_value(const short& type, const string& arg);

	void print_usage_info(const string& prog_name, cushort& indent = 1,
			ostream& out = cout) const;

	const vector<string>& get_types() const {
		return types;
	}

	void set_types(const vector<string>& types) {
		this->types = types;
	}

private:
	map<short, deque<Options>> options;
	map<short, deque<Switch>> switches;
	vector<string> types;
	static ushort name_width;
	static const string help_message;
};

namespace CMD {
Cmd_Line create_argument_list();
string create_version_info();
}
;

#endif /* CMD_HH_ */
