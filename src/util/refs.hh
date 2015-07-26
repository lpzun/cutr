/**
 * @name refs.hh
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#ifndef REFS_HH_
#define REFS_HH_

#include "state.hh"
#include "graph.hh"
namespace sura {

typedef unsigned int vertex;

class Refs {
public:
	Refs();
	~Refs();

	static bool OPT_PRINT_ALL;
	static bool OPT_PRINT_DOT;
	static bool OPT_SIMPLIFIED;
	static bool OPT_PRINT_CMD;
	static bool OPT_NOT_SIMPLE;
	static bool IS_BWS_TREE;
	static bool OPT_PRINT_PATH;
	static bool OPT_CONSTRAINT;
	static bool OPT_PRINT_ADJ;
	static bool OPT_COMPLETE;
	static bool OPT_BACKWARD;
	static bool OPT_SHARED;

	static ushort TARGET_THR_NUM;

//	static string FILE_NAME_PREFIX;

	static Thread_State INITL_TS;
	static Thread_State FINAL_TS;

	static adj_list original_TTD;
	static adj_list spawntra_TTD;

	static vector<Thread_State> mapping_TS;
	static map<Thread_State, id_thread_state> activee_TS;

	static uint INITL_N;
	static uint SPAWN_Z;

	/// global variable for elapsed time
	static clock_t ELAPSED_TIME;
};
} /* namespace SURA */

#endif /* REFS_HH_ */
