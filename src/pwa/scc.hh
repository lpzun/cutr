/**
 * @brief scc.hh
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#ifndef SCC_HH_
#define SCC_HH_

#include "ettd.hh"

namespace sura {
/// define SCC
class SCC {
public:
	SCC(const vertex& v);
	SCC(const vertex& v, const deque<vertex>& V);
	virtual ~SCC();

	bool is_trivial() const;
	bool is_nested() const;
	const vertex& get_v() const;
	const deque<edge>& get_E() const;
	const size_V size() const;

	ostream& to_stream(ostream& out = cout) const;
private:
	bool is_TRIVIAL;
	bool is_NESTED;

	size_V V_size;

	vertex v;       /// delegate
	deque<edge> E;   /// edges in SCC

	uint build_E(const deque<vertex>& V);
	bool is_loop_nests(const size_V& size, const uint& size_E);
};

inline ostream& SCC::to_stream(ostream& out) const {
	out << v << " ";
	return out;
}

/**
 * @brief overloading operator <<: print SCC
 * @param out
 * @param s
 * @return ostream
 */
inline ostream& operator<<(ostream& out, const SCC& s) {
	return s.to_stream(out);
}

/**
 * @brief overloading operator ==
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 == s2
 * 		   false: otherwise
 */
inline bool operator==(const SCC& s1, const SCC& s2) {
	return s1 == s2;
}

/**
 * @brief overloading operator !=
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 != s2
 * 		   false: otherwise
 */
inline bool operator!=(const SCC& s1, const SCC& s2) {
	return s1 != s2;
}

/**
 * @brief overloading operator <
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 < s2
 * 		   false: otherwise
 */
inline bool operator<(const SCC& s1, const SCC& s2) {
	return s1 < s2;
}

/**
 * @brief overloading operator <
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 < s2
 * 		   false: otherwise
 */
inline bool operator>(const SCC& s1, const SCC& s2) {
	return s1 > s2;
}

/// define SCC quotient graph
class GSCC {
public:
	GSCC(const size_V& V, const adj_list& Adj);
	virtual ~GSCC();

	const vector<_path>& find_all_paths() const;

	vector<shared_ptr<SCC>>& get_sccs() {
		return sccs;
	}
	vector<vector<shared_ptr<deque<edge>>> >&get_trans_btwn_sccs() {
		return trans_btwn_sccs;
	}
private:
	vector<shared_ptr<SCC>> sccs; /// store SCCs

	/// store transitions between SCCs
	vector<vector<shared_ptr<deque<edge>>> > trans_btwn_sccs;

	vector<_path> paths;/// all scc quotient paths

	vector<deque<vertex>> build_SCC(const size_V& V, const adj_list& Adj);
	void build_GSCC(const vector<deque<vertex>>& sccs);
	void build_E_in_GSCC(const deque<vertex>& scc1, const size_t& u,
			const deque<vertex>& scc2, const size_t& v, bool& is_uv, bool& is_vu);
};

}
/* namespace SURA */

#endif /* SCC_HH_ */
