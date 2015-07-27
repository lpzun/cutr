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
using map_expr = unordered_map<ushort, expr>;
/// Aliasing vector<expr> as vec_expr
using vec_expr = vector<expr>;

enum class result {
	reach = 0, unreach = 1, unknown = 2
};

class FWS {
public:
	FWS(const size_t& size_P, const shared_ptr<GSCC>& p_gscc);
	virtual ~FWS();

	bool fws_as_logic_decision(const vector<_path>& paths);

private:
	/// All expressions, func_decl, etc., appearing in the class must be
	/// defined in same context; otherwise, segmentation fault happens
	static context ctx;

	////// static variables used in the class to build the path constraint
	static string n_affix;
	static string k_affix;
	static string x_affix;

	static expr n_0;

	static uint max_n;  /// maximal number of initial   threads in max path
	static uint max_z;  /// maximal number of spawn transitions in max path

	static uint con_z;  /// the sum of constant spawn transitions in max path
	static expr sum_z;  /// expression of summarizing fired spawns in max path

	////// class member
	ushort k_index; /// index for loop iteration variables
	uint x_index;   /// index for marking equation variables

	shared_ptr<GSCC> p_gscc; /// pointer to GSCC: scc quotient graph
	vector<shared_ptr<solver>> solver_P; /// the vector of solvers for all paths
	vector<bool> sat_P;      /// mark which path is satisfiable
	set<id_tran> spaw_vars;  /// store the IDs of spawn transitions

	result quotient_path_reachability(const _path& P);
	result path_reachability(const vec_expr& phi, const ushort& id_P);
	//bool path_reachability(const _path& P, const ushort& id_P);

	////// build path constraint
//	vec_expr path_summary(const _path& P);
	vec_expr path_summary(const _path& path, const deque<size_t>& permu);

	void assemble(vec_expr &pfx, vec_expr &phi, const delta &delta);
	void assemble(vec_expr &pfx, vec_expr &phi, const delta &delta, const expr &k);
	void assemble(const vec_expr &pfx, vec_expr &phi, const vector<bool> &is_append);
	void assemble(const vec_expr &pfx, vec_expr &phi, const Shared_State &s_entr, const Shared_State &s_exit,
			const vector<bool> &is_append);

	vector<bool> append_marking_equation(vec_expr &pfx, const SCC &scc);

	result check_sat_via_smt_solver(shared_ptr<solver>& s);
	bool parse_sat_solution(const model& m);
	bool is_spawn_variable(const string& v);
	uint get_z3_const_uint(const expr& e);

	bool solicit_for_CEGAR();

	bool check_reach_with_fixed_threads(const uint& n, const uint& z);
	Locals update_counter(const Locals &Z, const ushort &inc);
	Locals update_counter(const Locals &Z, const Local_State &dec, const Local_State &inc);

	void reproduce_witness_path(const shared_ptr<Global_State>& pi, const set<Global_State> &R);
	bool is_initial_state(const Global_State &tau);
};

} /* namespace SURA */

#endif /* FWS_HH_ */
