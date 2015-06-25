/**===========================================================================================
 * @note        I am rewriting the URSULA on this project. I am trying to use more generic
 * 				programming techniques. This may spend some time. Not in a rush.
 *
 * @name       	: URSULA
 * 					Unbounded-thread Reachability via Symbolic execUtion and Loop Acceleration
 * @author     	: Peizun Liu
 * @version    	: v0.2
 * @copyright  	: The copy right belong to CAV group in CCIS NU
 * @brief		: We present a new approach to reachability analysis for communicating
 * 				finite-state threads that formulates the analysis as a logical decision
 * 				problem. In addition to the unbounded number of running threads, the
 * 				difficulty in making such approaches sound has traditionally been the
 * 				existence of unbounded loops in the program executed by a thread.
 * 				We show in this paper how loop iterations can be summarized into
 * 				Presburger arithmetic constraints. The constraints are defined over a
 * 				single-threaded abstract program and are satisfiable exactly if the original
 * 				unbounded-thread reachability problem has a solution. An advantage of
 * 				our approach is that the purely static analysis involved in building the
 * 				Presburger formula can often reveal its (un-)satisfiability, practically in
 * 				zero time, before any kind of arithmetic has been performed.
 * 				We demonstrate the power of this technique for checking safety properties
 * 				of unbounded-thread programs and infinite-state transition systems.
 *==========================================================================================*/

#include <iostream>

#include "sura/fws/fws.hh"
#include "util/cmd.hh"

using namespace sura;
using namespace std;

/**
 * @brief: The main function
 * @param: argc
 * @param: argv
 * @return int
 * 		   exit code
 */
int main(const int argc, const char * const * const argv) {
	try {
		Cmd_Line cmd = CMD::create_argument_list();
		try {
			cmd.get_command_line(argc, argv);
		} catch (Cmd_Line::Help) {
			return 0;
		}

		return 0;
	} catch (const sura_exception & e) {
		e.what();
	} catch (const z3::exception &e) {
		std::cerr << e.msg() << endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << endl;
	} catch (...) {
		std::cerr << sura_exception("main: unknown exception occurred").what() << endl;
	}
}
