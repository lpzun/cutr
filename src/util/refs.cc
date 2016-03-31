/**
 * @name refs.cc
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#include "refs.hh"

namespace sura {

Refs::Refs() {

}

Refs::~Refs() {

}

bool Refs::OPT_SIMPLIFIED = false;
bool Refs::OPT_NOT_SIMPLE = false;

bool Refs::IS_BWS_TREE = false;
bool Refs::OPT_COMPLETE = false;
bool Refs::OPT_BACKWARD = false;
bool Refs::OPT_SHARED = false;
bool Refs::OPT_PRINT_PATH = false;
bool Refs::OPT_CONSTRAINT = false;

bool Refs::OPT_PRINT_DOT = false;
bool Refs::OPT_PRINT_ADJ = false;
bool Refs::OPT_PRINT_CMD = false;
bool Refs::OPT_PRINT_ALL = false;

ushort Refs::TARGET_THR_NUM = 1; /// default is 1

//string Refs::FILE_NAME_PREFIX = "";

thread_state Refs::INITL_TS;
thread_state Refs::FINAL_TS;

adj_list Refs::original_TTD;
adj_list Refs::spawntra_TTD;

vector<thread_state> Refs::mapping_TS;
map<thread_state, id_thread_state> Refs::activee_TS;

clock_t Refs::ELAPSED_TIME = clock();

uint Refs::INITL_N = 1;
uint Refs::SPAWN_Z = 0;

} /* namespace SURA */
