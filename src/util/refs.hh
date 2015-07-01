/*
 * refs.hh
 *
 *  Created on: Jun 21, 2015
 *      Author: lpzun
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

extern const string VERSION;
extern const string OPT_MODE_FWS;
extern const string OPT_MODE_LDP;
extern const string OPT_MODE_CON;

extern string FILE_NAME_PREFIX;

extern Thread_State INITL_TS;
extern Thread_State FINAL_TS;

extern adj_list original_TTD;
extern adj_list spawntra_TTD;

} /* namespace SURA */

#endif /* REFS_HH_ */
