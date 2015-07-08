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

class Transition {
public:
	vertex src; /// source      of transition
	vertex dst; /// destination of transition

	type_T type; /// type of transition

	inline Transition(const vertex& src, const vertex& dst);
	inline Transition(const vertex& src, const vertex& dst, const type_T& type);
	virtual ~Transition(){
	}

	ostream& to_stream(ostream& out = cout) const;
};

inline Transition::Transition(const vertex& src, const vertex& dst) :
		src(src), dst(dst), type(NORM) {
}

inline Transition::Transition(const vertex& src, const vertex& dst, const type_T& type) :
		src(src), dst(dst), type(type) {
}

inline ostream& Transition::to_stream(ostream& out) const {
	out << mapping_TS[src] << " ";
	switch (type) {
	case EXPD:
		out << ":>";
		break;
	case THCR:
		out << "+>";
		break;
	case BCST:
		out << "~>";
		break;
	default:
		out << "->";
		break;
	}
	out << " " << mapping_TS[dst];
	out << "\n";
	return out;
}

/**
 * @brief overloading operator <<: print transition
 * @param out
 * @param ts
 * @return ostream
 */
inline ostream& operator<<(ostream& out, const Transition& r) {
	return r.to_stream(out);
}

/**
 * @brief overloading operator ==
 * @param r1
 * @param r2
 * @return bool
 * 		   true : if r1 == r2
 * 		   false: otherwise
 */
inline bool operator==(const Transition& r1, const Transition& r2) {
	return r1.src == r2.src && r1.dst == r2.dst;
}

/**
 * @brief overloading operator !=
 * @param r1
 * @param r2
 * @return bool
 * 		   true : if r1 != r2
 * 		   false: otherwise
 */
inline bool operator!=(const Transition& r1, const Transition& r2) {
	return !(r1 == r2);
}

/**
 * @brief overloading operator <
 * @param r1
 * @param r2
 * @return bool
 * 		   true : if r1 < r2
 * 		   false: otherwise
 */
inline bool operator<(const Transition& r1, const Transition& r2) {
	if (r1.src == r2.src)
		return r1.dst < r2.dst;
	return r1.src < r2.src;
}

/**
 * @brief overloading operator >
 * @param r1
 * @param r2
 * @return bool
 * 		   true : if r1 > r2
 * 		   false: otherwise
 */
inline bool operator>(const Transition& r1, const Transition& r2) {
	if (r1.src == r2.src)
		return r1.dst > r2.dst;
	return r1.src > r2.src;
}

///////////////////////////////////////////////////////////////////////////////

typedef Transition edge; /// rename transition as edge

class ETTD {
public:
	static ETTD& instance();

	virtual ~ETTD();

	adj_list get_expansion_TTD();
	list<edge> get_transitions();

	void build_SCC();

	void print_expansion_TTD();
	void print_transitions();

private:
	ETTD();
	ETTD(const adj_list& TTD);

	static unique_ptr<ETTD> m_instance;

	adj_list expanded_TTD;
	list<edge> transitions;

	void build_ETTD(const adj_list& TTD);
	bool is_horizontal(const vertex& u, const vertex& v);
	bool is_real_trans(const vertex& u, const vertex& v, const adj_list& TTD);

	vector<bool> find_expansion_trans_dest(const vertex& v,
			const adj_list& TTD);
	void build_SCC(const size_V& V, const adj_list& Adj);
};
} /* namespace SURA */

#endif /* ETTD_HH_ */
