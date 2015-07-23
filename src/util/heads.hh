/*
 * heads.hh
 *
 *  Created on: Jun 25, 2015
 *      Author: lpzun
 */

#ifndef HEADS_HH_
#define HEADS_HH_
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <ctime>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <cctype>
#include <list>
#include <stack>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <unordered_map>
#include <queue>
#include <utility>
#include <iterator>
#include <algorithm>
#include <memory>

#include "excep.hh"
#include "debug.hh"

using std::string;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;

using std::list;
using std::set;
using std::stack;
using std::vector;
using std::map;
using std::deque;
using std::pair;
using std::unordered_map;
using std::queue;
using std::multimap;

using std::shared_ptr;
using std::unique_ptr;

typedef const short cshort;
typedef const int cint;
typedef const long clong;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef const unsigned short cushort;
typedef const unsigned int cuint;
typedef const unsigned long culong;

//// some macro definition

/// Evaluate assertion if __SAFE_COMPUTATION__ is defined.
/// If not, __SAFE_ASSERT__ results in an empty statement ";".
/// Careful: in macro definition, NO SPACE between macro name
/// 		 and formal arg list!
#ifdef __SAFE_COMPUTATION__
#define __SAFE_COMPUTE__(stmt) stmt
#define __SAFE_ASSERT__(cond) assert(cond)  // an abbreviation for: __SAFE_COMPUTE__ (assert cond)
#else
#define __SAFE_COMPUTE__(stmt)
#define __SAFE_ASSERT__(cond)
#endif

#ifdef __VERBOSE__
#define __VERBOSE_COMPUTE__(stmt) stmt
#define __CERR__(stuff) std::cerr << stuff  // print only in verbose mode. Use for debugging messages
#else
#define __VERBOSE_COMPUTE__(stmt)
#define __CERR__(stuff)
#endif

#endif /* HEADS_HH_ */
