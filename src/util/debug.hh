/**
 * @brief using it for debugging
 *
 * @name debug.hh
 *
 * @date  : Oct 2, 2013
 * @author: Peizun Liu
 */

#ifndef DEBUG_HH_
#define DEBUG_HH_

//#define DEBUG_MODE
//#define _STATISTICS_

#ifndef NDEBUG
#define NDEBUG
#else

#endif

#ifdef DEBUG_MODE
#  define DBG_LOG(x) std::cerr << x <<"\n";
#  define DBG_STD(x) x;
#  define DBG_LOC() std::cerr <<__func__<<" I am here ..."<<"\n";
#else
#  define DBG_LOG(x)
#  define DBG_STD(x)
#endif // DEBUG

class Debug {
public:
	Debug();
	virtual ~Debug();
};

#endif /* DEBUG_HH_ */
