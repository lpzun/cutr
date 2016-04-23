/******************************************************************************
 *  @name cmd.cc
 *
 *  @date  : Nov 18, 2014
 *  @author: Peizun Liu
 *****************************************************************************/

#include "cmd.hh"

cmd_line::cmd_line() :
		SHORT_HELP_OPT("-h"), LONG_HELP_OPT("--help"), SHORT_VERSION_OPT("-v"), LONG_VERSION_OPT(
				"--version"), VERSION("v1.0"), name_width(0), help_message(
				"Use " + string(SHORT_HELP_OPT) + " or " + string(LONG_HELP_OPT)
						+ " for help"), v_info(), options(), switches(), types() {
	this->create_argument_list();
}

cmd_line::~cmd_line() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param argc
 * @param argv
 */
void cmd_line::get_command_line(const int argc,
		const char* const * const argv) {
	vector<string> args(argc - 1);
	for (int i = 0; i < argc - 1; i++) {
		args[i] = argv[i + 1];
	}
	this->get_command_line(argv[0], args);
}

/**
 * @brief Parsing command line
 * @param prog
 * @param args
 */
void cmd_line::get_command_line(const string& prog,
		const vector<string>& args) {
	for (auto iarg = args.begin(), iend = args.end(); iarg != iend; ++iarg) {
		const string& arg = *iarg;
		if (arg == SHORT_HELP_OPT || arg == LONG_HELP_OPT) {
			this->print_usage_info(prog);
			throw Help();
		} else if (arg == SHORT_VERSION_OPT || arg == LONG_VERSION_OPT) {
			cout << v_info << endl;
			throw Help();
		} else {
			bool flag = true;
			for (ushort i = 0; i < types.size() && flag; ++i) {
				auto iopts = options.find(i);
				if (iopts != options.end())
					for (auto iopt = iopts->second.begin();
							iopt != iopts->second.end() && flag; ++iopt) {
						if (arg == iopt->get_short_name()
								|| arg == iopt->get_long_name()) {
							++iarg;   /// the next string is the value for arg
							iopt->set_value(*iarg);
							if (flag)
								flag = false;
						}
					}

				auto iswts = switches.find(i);
				if (iswts != switches.end())
					for (auto iswt = iswts->second.begin();
							iswt != iswts->second.end() && flag; ++iswt) {
						if (arg == iswt->get_short_name()
								|| arg == iswt->get_long_name()) {
							iswt->set_value(true);
							if (flag)
								flag = false;
						}
					}

			}
			if (flag)
				throw ural_rt_err(
						"Cmd_Line::get_command_line: " + arg
								+ ": no such keyword argument.\n"
								+ help_message);
		}
	}
}

/**
 *
 * @param type
 * @param short_name
 * @param long_name
 * @param meaning
 * @param default_value
 */
void cmd_line::add_option(const short& type, const string& short_name,
		const string& long_name, const string& meaning,
		const string& default_value) {
	this->options[type].push_back(
			Options(type, short_name, long_name, meaning, default_value));
	this->name_width =
			name_width < short_name.size() + long_name.size() + 5 ?
					short_name.size() + long_name.size() + 5 : name_width;
}

/**
 *
 * @param type
 * @param short_name
 * @param long_name
 * @param meaning
 */
void cmd_line::add_switch(const short& type, const string& short_name,
		const string& long_name, const string& meaning) {
	this->switches[type].push_back(
			Switch(type, short_name, long_name, meaning));
	this->name_width =
			name_width < short_name.size() + long_name.size() + 5 ?
					short_name.size() + long_name.size() + 5 : name_width;
}

/**
 * @brief return bool value
 * @param type
 * @param arg
 * @return bool
 */
bool cmd_line::arg_bool(const unsigned short& type, const string& arg) {
	auto ifind = std::find(switches[type].begin(), switches[type].end(),
			Switch(type, arg));
	if (ifind == switches[type].end())
		throw ural_rt_err("Cmd_Line:: argument " + arg + " does not exist!"); //TODO add
	return ifind->is_value();
}

/**
 * @brief return the value of argument
 * @param type
 * @param arg
 * @return string: the value of argument
 */
string cmd_line::arg_value(const short& type, const string& arg) {
	auto ifind = std::find(options[type].begin(), options[type].end(),
			Options(type, arg));
	if (ifind == options[type].end())
		throw ural_rt_err("Cmd_Line:: argument " + arg + " does not exist!"); //TODO add
	return ifind->get_value();
}

/**
 * @brief print help information
 * @param prog_name
 * @param indent
 * @param out
 */
void cmd_line::print_usage_info(const string& prog_name, cushort& indent,
		ostream& out) const {
	out << "\n";
	out << v_info << "\n";

	out << PPRINT::widthify("Usage:", this->name_width, PPRINT::LEFTJUST)
			<< PPRINT::widthify("Purpose:", PPRINT::LEFTJUST) << "\n";
	out << " "
			<< PPRINT::widthify(
					prog_name + " " + SHORT_HELP_OPT + " [" + LONG_HELP_OPT
							+ "]", this->name_width, PPRINT::LEFTJUST)
			<< PPRINT::widthify("show help message", PPRINT::LEFTJUST) << "\n";
	out << " "
			<< PPRINT::widthify(prog_name + " source.ttd ", this->name_width,
					PPRINT::LEFTJUST)
			<< PPRINT::widthify("check given program", PPRINT::LEFTJUST)
			<< "\n";

	for (size_t i = 0; i < types.size(); i++) {
		out << types[i] << "\n";
		auto iopts = options.find(i);
		if (iopts != options.end())
			for (auto iopt = iopts->second.begin(); iopt != iopts->second.end();
					++iopt) {
				out << " "
						<< PPRINT::widthify(
								iopt->get_short_name() + " ["
										+ iopt->get_long_name() + "] arg",
								this->name_width, PPRINT::LEFTJUST)
						<< PPRINT::widthify(
								PPRINT::widthify(iopt->get_meaning(),
										this->name_width + 2), PPRINT::LEFTJUST)
						<< "\n";
			}

		auto iswts = switches.find(i);
		if (iswts != switches.end())
			for (auto iswt = iswts->second.begin(); iswt != iswts->second.end();
					++iswt) {
				out << " "
						<< PPRINT::widthify(
								iswt->get_short_name() + " ["
										+ iswt->get_long_name() + "]",
								this->name_width, PPRINT::LEFTJUST)
						<< PPRINT::widthify(iswt->get_meaning(),
								PPRINT::LEFTJUST) << "\n";
			}

		out << endl;
	}
}

void cmd_line::create_argument_list() {
	vector<string> types(opts_types(), "");
	types[1] = "Problem instance:";
	types[2] = "Exploration mode:";
//	types[3] = "SMT Solver options:";
	types[3] = "Other options:";

	v_info = create_version_info();

	this->set_types(types);
	this->add_switch(default_opts(), SHORT_HELP_OPT, LONG_HELP_OPT,
			"help information");
	this->add_switch(default_opts(), "-s", "--statistics",
			"show statistic information");

	/// problem instance
	this->add_option(prob_inst_opts(), "-f", "--input-file",
			"thread-state transition diagram (.ttd file)", "X");
	this->add_option(prob_inst_opts(), "-a", "--target",
			"a target thread state (e.g., 0|0)", "0|0");
	this->add_option(prob_inst_opts(), "-i", "--initial",
			"an initial thread state (e.g., 0|0)", "0|0");

	this->add_switch(prob_inst_opts(), "-l", "--adj-list",
			"show the adjacency list");
	this->add_switch(prob_inst_opts(), "-dot", "--ettd2dot",
			"output generated expanded TTD to a .dot file");

	/// exploration mode
	this->add_option(exp_mode_opts(), "-m", "--mode",
			(string("exploration mode:\n") //
					///+ string(25, ' ') + " -" + '"' + opt_mode_fws() + '"' + ": forward search: oracle\n" //
					+ string(25, ' ') + " -" + '"' + opt_mode_ldp() + '"'
					+ ": logic decision problem\n" //
			///		+ string(25, ' ') + " -" + '"' + opt_mode_con() + '"' + ": concurrent forward/logic decision\n" //
			).c_str(), opt_mode_ldp());

	this->add_switch(exp_mode_opts(), "-t", "--bwstree",
			"show backward search tree");
	this->add_switch(exp_mode_opts(), "-p", "--path",
			"show all paths in SCC quotient graph");
	this->add_switch(exp_mode_opts(), "-cmp", "--complete",
			"verification via a complete approach");
	this->add_switch(exp_mode_opts(), "-bws", "--backward",
			"backward search based encoding (default=false)");
	this->add_switch(exp_mode_opts(), "-sh", "--shared",
			"shared states based constraints (default=false)");

	/// SMT Solver options
	//DBG_STD(cmd.add_option(SMT_SOLVER_OPTS, "-smt", "--smt-solver", "set the SMT Solver to be used", "z3"));

	/// other options
	this->add_switch(other_opts(), "-ce", "--counterexample",
			"show counterexample");
	this->add_switch(other_opts(), "-ns", "--nosimpl",
			"simplify the generated Presburger formula");
	this->add_switch(other_opts(), "-cs", "--cstr",
			"output intermediate constraints");

	this->add_switch(other_opts(), "-cmd", "--cmd-line",
			"show the command line");
	this->add_switch(other_opts(), "-all", "--all",
			"show all of above messages");
	this->add_switch(other_opts(), SHORT_VERSION_OPT, LONG_VERSION_OPT,
			"show version information and exit");
}

string cmd_line::create_version_info() {
	string info = ""; ///
	info ///
	.append("* *      _/_/_/    _/    _/    _/_/_/_/_/    _/_/_/     * *\n") ///
	.append("* *    _/         _/    _/        _/        _/    _/    * *\n") ///
	.append("* *   _/         _/    _/        _/        _/_/_/       * *\n") ///
	.append("* *  _/         _/    _/        _/        _/ _/         * *\n") ///
	.append("* *   _/_/_/   _/_/_/_/        _/        _/    _/  "+ VERSION + " * *\n") ///
	.append("-----------------------------------------------------------\n") ///
	.append("* *        Concolic Unbounded-Thread Reachability       * *\n") ///
	.append("* *                 via Loop Summaries                  * *\n")	///
	.append("* *           Peizun Liu @ Thomas Wahl's Group          * *\n") ///
	.append("* *        Northeastern University, United States       * *\n") ///
	.append("* *                             Build Time: ").append(__DATE__).append(" * *\n") ///
	.append(
			"-----------------------------------------------------------\n");
	return info;
}
