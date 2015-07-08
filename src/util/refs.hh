/**
 * @brief refs.hh
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#ifndef REFS_HH_
#define REFS_HH_

#include "graph.hh"
#include "state.hh"

namespace sura {
extern bool OPT_PRINT_ALL;
extern bool OPT_PRINT_DOT;
extern bool OPT_SIMPLIFIED;
extern bool OPT_PRINT_CMD;
extern bool OPT_NOT_SIMPLE;
extern bool IS_BWS_TREE;
extern bool OPT_PRINT_PATH;
extern bool OPT_CONSTRAINT;
extern bool OPT_PRINT_ADJ;
extern bool OPT_COMPLETE;
extern bool OPT_BACKWARD;
extern bool OPT_SHARED;

extern ushort TARGET_THR_NUM;

extern string FILE_NAME_PREFIX;

extern Thread_State INITL_TS;
extern Thread_State FINAL_TS;

extern adj_list original_TTD;
extern adj_list spawntra_TTD;

extern vector<Thread_State> mapping_TS;

/// store all incoming vertices to   same shared state
/// key is the shared state, value is the set of incoming vertices
extern map<Shared_State, set<vertex>> s_incoming;

/// store all outgoing vertices from same shared state
/// key is the shared state, value is the set of outgoing vertices
extern map<Shared_State, set<vertex>> s_outgoing;

/// global variable for elapsed time
extern clock_t ELAPSED_TIME;

} /* namespace SURA */

#endif /* REFS_HH_ */
