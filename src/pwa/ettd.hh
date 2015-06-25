/**
 * @brief ettd.hh
 * @date: Jun 21, 2015
 * @author: lpzun
 */

#ifndef ETTD_HH_
#define ETTD_HH_

#include "../util/refs.hh"

namespace sura {

typedef enum Transition_Type {
	NORM, THCR, BCST, EXPD
} type_T;

template<typename T> class Transition {
public:
	T src; /// source      of transition
	T dst; /// destination of transition

	type_T type; /// type of transition

	Transition(const T& src, const T& dst);
	Transition(const T& src, const T& dst, const type_T& type);
	virtual ~Transition();
};

typedef list<Transition<ushort>> edge;
typedef unsigned int vertex;

class ETTD {
public:
	static ETTD& instance();
	virtual ~ETTD();

	adj_list get_expansion_TTD();
	list<edge> get_transitions();
	static unique_ptr<ETTD> m_instance;

private:
	ETTD();
	ETTD(const adj_list& TTD);

	adj_list expansion_TTD;
	list<edge> transitions;

	void build_ETTD(const adj_list& TTD);

};
} /* namespace SURA */

#endif /* ETTD_HH_ */
