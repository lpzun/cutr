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

/////////////////// command line class
class cmd_line {
public:
	cmd_line();
	virtual ~cmd_line();

	struct Help {
	};

	void get_command_line(const int argc, const char* const * const argv);
	bool arg_bool(const unsigned short& type, const string& arg);
	string arg_value(const short& type, const string& arg);

	const vector<string>& get_types() const {
		return types;
	}

	void set_types(const vector<string>& types) {
		this->types = types;
	}

	inline static unsigned short opts_types() {
		static unsigned short OPTS_TYPES = 4;
		return OPTS_TYPES;
	}

	inline static unsigned short default_opts() {
		static unsigned short DEFAULT_OPTS = 0;
		return DEFAULT_OPTS;
	}

	inline static unsigned short prob_inst_opts() {
		static unsigned short PROB_INST_OPTS = 1;
		return PROB_INST_OPTS;
	}

	inline static unsigned short exp_mode_opts() {
		static unsigned short EXP_MODE_OPTS = 2;
		return EXP_MODE_OPTS;
	}

	inline static unsigned short smt_solver_opts() {
		static unsigned short SMT_SOLVER_OPTS = 4;
		return SMT_SOLVER_OPTS;
	}

	inline static unsigned short other_opts() {
		static unsigned short OTHER_OPTS = 3;
		return OTHER_OPTS;
	}

	inline static const string& opt_mode_bws() {
		static string  OPT_MODE_CON= "B";
		return OPT_MODE_CON;
	}

	inline static const string& opt_mode_fws() {
		static string OPT_MODE_FWS = "F";
		return OPT_MODE_FWS;
	}

private:
	string SHORT_HELP_OPT;
	string LONG_HELP_OPT;
	string SHORT_VERSION_OPT;
	string LONG_VERSION_OPT;

	string VERSION;

	ushort name_width;
	const string help_message;
	string v_info;

	map<short, deque<Options>> options;
	map<short, deque<Switch>> switches;
	vector<string> types;

	void create_argument_list();
	string create_version_info();

	void add_option(const short& type, const string& short_name,
			const string& long_name, const string& meaning,
			const string& default_value);

	void add_switch(const short& type, const string& short_name,
			const string& long_name, const string& meaning);

	void get_command_line(const string& prog, const vector<string>& args);

	void print_usage_info(const string& prog_name, cushort& indent = 1,
			ostream& out = cout) const;
};

#endif /* CMD_HH_ */
