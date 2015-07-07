/**
 * @brief refs.cc
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#include "refs.hh"

namespace sura {
bool OPT_SIMPLIFIED = false;
bool OPT_NOT_SIMPLE = false;

bool IS_BWS_TREE = false;
bool OPT_COMPLETE = false;
bool OPT_BACKWARD = false;
bool OPT_SHARED = false;
bool OPT_PRINT_PATH = false;
bool OPT_CONSTRAINT = false;

bool OPT_PRINT_DOT = false;
bool OPT_PRINT_ADJ = false;
bool OPT_PRINT_CMD = false;
bool OPT_PRINT_ALL = false;

ushort TARGET_THR_NUM = 1; /// default is 1

string FILE_NAME_PREFIX = "";

Thread_State INITL_TS;
Thread_State FINAL_TS;

adj_list original_TTD;
adj_list spawntra_TTD;

vector<Thread_State> mapping_TS;
map<vertex, set<vertex>> s_incoming;
map<vertex, set<vertex>> s_outgoing;

clock_t ELAPSED_TIME = clock();
} /* namespace SURA */
