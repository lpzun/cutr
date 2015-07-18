/**
 * @brief fws.cc
 *
 * @date  : Jun 25, 2015
 * @author: Peizun Liu
 */

#ifndef FWS_HH_
#define FWS_HH_

#include "z3++.h"

#include "../../pwa/scc.hh"
#include "../ufun.hh"

using namespace z3;

namespace sura {

/// Aliasing unordered_map<ushort, expr> as map_expr
typedef unordered_map<ushort, expr> map_expr;
/// Aliasing vector<expr> as vec_expr
typedef vector<expr> vec_expr;
/// Aliasing list<expr>   as lst_expr
typedef list<expr> lst_expr;

/// alias the node in a SCC quotient path
/// key  : current node, i.e., SCC
/// value: edges leads to next node
typedef pair<SCC, list<edge>> sigma_v;

/// alias a SCC quotient path
typedef list<sigma_v> sigma;

class FWS {
public:
	FWS(const size_t& size_P, const shared_ptr<GSCC>& p_gscc);
	virtual ~FWS();

	bool fws_as_logic_decision(const vector<_path>& paths);

private:
	/// All expressions, func_decl, etc., appearing in the class must be
	/// defined in same context; otherwise, segmentation fault happens
	static context ctx;

	/// global variables used in the class to build the path constraint
	static string n_affix;
	static string k_affix;
	static ushort k_index;
	static string x_affix;
	static uint x_index;

	static expr n_0;

	static uint max_n;  /// maximal number of initial   threads in max path
	static uint max_z;  /// maximal number of spawn transitions in max path

	static uint con_z;  /// the sum of constant spawn transitions in max path
	static expr sum_z;  /// expression of summarizing fired spawns in max path

	shared_ptr<GSCC> p_gscc;

	/// mark which path is satisfiable
	vector<bool> sat_P;
	/// the vector of solvers of for all paths
	vector<solver> solver_P;
	/// store the IDs of spawn transitions
	set<id_tran> spaw_vars;

	bool path_reachability(const _path& P, const ushort& id_P);

	/// build path constraint
	vec_expr path_summary(const _path& P);

	bool check_sat_via_smt_solver(solver& s);
	void parse_sat_solution(const model& m);
	bool is_spawn_variable(const string& v);
	uint get_z3_const_uint(const expr& e);

	bool solicit_for_CEGAR();

	bool check_reach_with_fixed_threads(const uint& n, const uint& z);
	Locals update_counter(const Locals &Z, const ushort &inc);
	Locals update_counter(const Locals &Z, const Local_State &dec,
			const Local_State &inc);

//	void reset_spawn_expr();
//	void build_spawn_expr();
};

} /* namespace SURA */

#endif /* FWS_HH_ */
