/*
 * refs.cc
 *
 *  Created on: Jun 21, 2015
 *      Author: lpzun
 */

#include "refs.hh"

namespace sura {
bool OPT_SIMPLIFIED = false;
bool OPT_NOT_SIMPLE = false;

//bool IS_BWS_TREE = false;
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

const string OPT_MODE_FWS = "F";
const string OPT_MODE_LDP = "S";
const string OPT_MODE_CON = "L";

string FILE_NAME_PREFIX = "";

Thread_State INITL_TS;
Thread_State FINAL_TS;

const string VERSION = "v1.0";

adj_list original_TTD;
adj_list spawntra_TTD;

} /* namespace SURA */
