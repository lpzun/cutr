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

//#define NDEBUG

#ifndef NDEBUG
#define DEBUG_MODE
#else
#endif

#ifdef DEBUG_MODE
#  define DBG_LOG(x) std::cerr << x <<"\n";
#  define DBG_STD(x) x;
#  define DBG_LOC() std::cerr <<__func__<<" I am here ..."<<"\n";
#else
#  define DBG_LOG(x)
#  define DBG_STD(x)
#  define DBG_LOC();
#endif // DEBUG

class Debug {
public:
	Debug();
	virtual ~Debug();
};

#endif /* DEBUG_HH_ */
