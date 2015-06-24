/**===========================================================================================
 * @note        I am rewriting the ursula on this project. I am tring to use more generic
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
 * 				our approach is that the purely static analyses involved in building the
 * 				Presburger formula can often reveal its (un-)satisfiability, practically in
 * 				zero time, before any kind of arithmetic has been performed.
 * 				We demonstrate the power of this technique for checking safety properties
 * 				of unbounded-thread programs and infinite-state transition systems.
 *==========================================================================================*/

#include <iostream>
using namespace std;

/**
 * @brief
 * @param argc
 * @param argv
 * @return
 */
int main(const int argc, const char * const * const argv) {

	return 0;
}
