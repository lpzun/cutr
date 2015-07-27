/**
 * @name ettd.hh
 * @date  : Jun 21, 2015
 * @author: Peizun Liu
 */

#ifndef ETTD_HH_
#define ETTD_HH_

#include "../util/refs.hh"

namespace sura {

typedef unsigned int id_tran;

class Transition {
public:
	static id_tran ID;

	inline Transition(const Transition& t);
	inline Transition(const vertex& src, const vertex& dst);
	inline Transition(const vertex& src, const vertex& dst, const id_tran& id);
	virtual ~Transition() {
	}

	ostream& to_stream(ostream& out = cout) const;

	inline vertex get_src() const {
		return src;
	}

	inline vertex get_dst() const {
		return dst;
	}

	inline id_tran get_id() const {
		return id;
	}

private:
	vertex src; /// source      of transition
	vertex dst; /// destination of transition
	id_tran id;
};

inline Transition::Transition(const Transition& t) :
		src(t.src), dst(t.dst), id(t.id) {

}

inline Transition::Transition(const vertex& src, const vertex& dst) :
		src(src), dst(dst), id(0) {
}

inline Transition::Transition(const vertex& src, const vertex& dst, const id_tran& id) :
		src(src), dst(dst), id(id) {

}

inline ostream& Transition::to_stream(ostream& out) const {
	out << Refs::mapping_TS[src] << " ";
//	switch (type) {
//	case type_T::EXPD:
//		out << ":>";
//		break;
//	case type_T::THCR:
//		out << "+>";
//		break;
//	case type_T::BCST:
//		out << "~>";
//		break;
//	default:
//		out << "->";
//		break;
//	}
	out << "->";
	out << " " << Refs::mapping_TS[dst];
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
	return r1.get_src() == r2.get_src() && r1.get_dst() == r2.get_dst();
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
	if (r1.get_src() == r2.get_src())
		return r1.get_dst() < r2.get_dst();
	return r1.get_src() < r2.get_src();
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
	if (r1.get_src() == r2.get_src())
		return r1.get_dst() > r2.get_dst();
	return r1.get_src() > r2.get_src();
}

///////////////////////////////////////////////////////////////////////////////

typedef Transition edge; /// rename transition as edge
typedef vector<vector<bool>> matrix;
typedef pair<set<vertex>, set<vertex>> inout;

class ETTD {
public:
	static matrix real_R;
	static matrix spaw_R;
	static matrix expd_R;

	ETTD(const adj_list& TTD, const vector<inout>& s_in_out);
	virtual ~ETTD();

	void print_expanded_TTD();
	void print_transitions();

	size_V get_V() const;
	const adj_list& get_expanded_TTD() const;

private:
	size_V V;
	adj_list expanded_TTD;

	void build_ETTD(const adj_list& TTD, const vector<inout>& s_in_out);
	void build_real_R();
	bool is_horizontal(const vertex& u, const vertex& v);
	bool is_real_trans(const vertex& u, const vertex& v);
};
}
/* namespace SURA */

#endif /* ETTD_HH_ */
