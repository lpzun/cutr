/**
 * @brief fws.cc
 *
 * @date  : Jun 25, 2015
 * @author: Peizun Liu
 */

#include "fws.hh"

namespace sura {

/////////////////////// class members defined here ////////////////////////////

/// All expressions, func_decl, etc., appearing in the class must be
/// defined in same context; otherwise, segmentation fault happens
context FWS::ctx;

/// global variables used in the class to build the path constraint
string FWS::n_affix = "n"; /// prefix for variables of local state counters

string FWS::k_affix = "k"; /// prefix for loop iteration variables
ushort FWS::k_index = 0;   /// index for loop iteration variables
string FWS::x_affix = "x"; /// prefix for marking equation variables
uint FWS::x_index = 0;     /// index for marking equation variables

/// counter variable for initial local state
expr FWS::n_0 = ctx.int_const(
		(n_affix + std::to_string(INITL_TS.get_local())).c_str());

uint FWS::max_n = 0;  /// maximal number of initial   threads in max path
uint FWS::max_z = 0;  /// maximal number of spawn transitions in max path
uint FWS::con_z = 0;  /// the sum of constant spawn transitions in max path

/// expression of summarizing fired spawns in max path
expr FWS::sum_z = ctx.int_val(max_z);

////////////////////////// function members defined here //////////////////////
/**
 * @brief FWS constructor
 * @param size_P: the number of SCC quotient paths
 * @param p_gscc: the pointer to SCC quotient graph
 */
FWS::FWS(const size_t& size_P, const shared_ptr<GSCC>& p_gscc) :
		p_gscc(p_gscc), sat_P(vector<bool>(size_P, false)), solver_P(
				vector<solver>(size_P,
						(tactic(ctx, "simplify") & tactic(ctx, "qe")
								& tactic(ctx, "smt")).mk_solver())), spaw_vars(
				set<id_tran>()) {
}

FWS::~FWS() {
}

/**
 * @brief forward-based symbolic reachability analysis
 * @param paths
 * @return bool
 * 		true : if final is reachable
 * 		false: otherwise
 */
bool FWS::fws_as_logic_decision(const vector<_path>& paths) {
	bool is_existing_sat_P = false;
	for (auto idx = 0; idx < paths.size(); ++idx) {
		if (this->path_reachability(paths[idx], idx)) {
			sat_P[idx] = true;
			if (!is_existing_sat_P)
				is_existing_sat_P = true;
		}
	}

	if (!is_existing_sat_P)
		return false;
	return this->solicit_for_CEGAR();
}

/**
 * @brief encoding a path as a Presburger formula: forward version
 * @param final
 * @param path
 * @return bool
 * 		true : if the encoding formula is satisfiable
 * 		false: otherwise
 */
bool FWS::path_reachability(const _path& P, const ushort& id_P) {
	auto phi = this->path_summary(P);
	/// add k_i >= 0
	for (auto idx = 0; idx < k_index; ++idx)
		solver_P[id_P].add(
				ctx.int_const((k_affix + std::to_string(idx)).c_str()) >= 0);

	/// add x_i >= 0
	for (auto idx = 0; idx < x_index; ++idx)
		solver_P[id_P].add(
				ctx.int_const((x_affix + std::to_string(idx)).c_str()) >= 0);

	/// add constraints
	for (auto iphi = phi.begin(); iphi != phi.end(); ++iphi)
		solver_P[id_P].add(*iphi);

	return this->check_sat_via_smt_solver(solver_P[id_P]);
}

/**
 * @brief encoding a path as a Presburger formula: forward version
 * @param p: a path
 * @return vec_expr
 * 		a vector of constraint for all local and shared states
 */
vec_expr FWS::path_summary(const _path& P) {
	/// pfx: The vector of expressions: store prefix formula
	/// 	 Range [0, l) store constraints for local states
	///		 Range [l, l+s) store constraints for shared states
	vec_expr pfx(Thread_State::L + Thread_State::S, ctx.int_val(0));
	pfx[INITL_TS.get_local()] = n_0;

	/// phi: The vector of expressions: store path summary constraints
	/// 	 Range [0, l) store constraints for local states
	///		 Range [l, l+s) store constraints for shared states
	vec_expr phi(Thread_State::L + Thread_State::S, ctx.bool_val(true));
	phi[INITL_TS.get_local()] = n_0 > 1; /// constraint n_0 > 1

	/// reset spawn expression
	sum_z = ctx.int_val(0);
	con_z = 0;

#ifndef NDEBUG
	cout << __func__ << endl;
	for (auto iscc = P.begin(); iscc != P.end(); ++iscc) {
		cout << " => " << *iscc << " by ";
		if (p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(iscc))] != nullptr)
			for (const auto& t : *(p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(
					iscc))]))
				cout << t << " ";
	}
	cout << endl;
#endif

	list<edge> infix;
	for (auto iscc = P.begin(); iscc != P.end(); ++iscc) {
		const auto p_scc = p_gscc->get_sccs()[*iscc];
		const auto p_trans = p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(
				iscc))];

		if (p_scc->is_trivial() == false) {
			const auto p_next = p_gscc->get_sccs()[*(std::next(iscc))];
			if (p_scc->is_nested() == false) { /// simple loop
				cout << "I am here: simple loop\n";
			} else {                        /// nested loop
			}
		} else {
			cout << "I am here: trivial SCC\n";
		}
	}

	/// build spawn expression
	sum_z = sum_z + ctx.int_val(con_z);

	cout << "I am at the end of path_summary" << endl;
	return phi;
}

/**
 * @brief checking if a constraint is satisfiable or not
 * @param s: a SMT solver
 * @return bool
 * 		true : if s is satisfiable
 * 		false: otherwise
 */
bool FWS::check_sat_via_smt_solver(solver& s) {
	switch (s.check()) {
	case sat:   /// if   sat
		this->parse_sat_solution(s.get_model());
		return true;
	case unsat: /// if unsat
		return false;
	case unknown:
		throw ural_rt_err("smt solver returns unknow!");
	}
}

/**
 * @brief parse the sat solution and see if it corresponds to a witness
 * @param m: the solution returned from z3 SMT solver
 * @return bool
 * 		   true : if it corresponds to a witness path
 * 		   false: otherwise
 */
void FWS::parse_sat_solution(const model& m) {
	uint n = 0;
	for (auto i = 0; i < m.size(); i++) {
		func_decl v = m[i];
		assert(v.arity() == 0); /// check if contains only constants
		if (v.name() == n_0.decl().name())
			if (Z3_get_numeral_uint(ctx, m.get_const_interp(v), &n))
				break;
	}

	if (max_n < n)
		max_n = n;

	auto z = get_z3_const_uint(m.eval(sum_z));
	if (max_z < z)
		max_z = z;
}

/**
 * @brief retrieve the value from a z3 constant expression
 * @param e: a constant expression
 * @return e's value
 */
uint FWS::get_z3_const_uint(const expr& e) {
	if (e.is_const()) {
		uint value = 0;
		Z3_get_numeral_uint(ctx, e, &value);
		return value;
	} else {
		throw ural_rt_err("get_z3_const_uint: input is not a constant");
	}
}

/**
 *
 * @param v
 * @return
 */
bool FWS::is_spawn_variable(const string& v) {
	return spaw_vars.find(std::stoi(v.substr(x_affix.size())))
			!= spaw_vars.end();
}

/**
 * @brief solicit for CEGAR to refine pathwise encoding
 * @param sat_P
 * @return bool
 * 		true : if we find a
 * 		false:
 */
bool FWS::solicit_for_CEGAR() {
	if (this->check_reach_with_fixed_threads(max_n, max_z))
		return true;

	bool is_existing_sat_P = true;
	for (auto idx = 0; idx < sat_P.size(); ++idx) {
		if (sat_P[idx] == true) {
			if (!is_existing_sat_P)
				is_existing_sat_P = true;
			/// add increment constraints
			solver_P[idx].add(n_0 > ctx.int_val(max_n));
			if (max_z > 0 && !sum_z.is_int())
				solver_P[idx].add(sum_z > ctx.int_val(max_z));

			/// apply incremental solving
			if (!check_sat_via_smt_solver(solver_P[idx])) {
				sat_P[idx] = false;
				is_existing_sat_P = false;
			}
		}
	}

	if (!is_existing_sat_P)
		return false;
	else
		return this->solicit_for_CEGAR();
}

/**
 * @brief This procedure checks the reachability of final thread state with
 * 		fixed number of threads, i.e., if there exists a path tau_0 ->* tau_F
 * 		s.t. tau_F covers final.
 * @param n: # of initial    threads
 * @param z: # of spawns transitions
 * @return
 */
bool FWS::check_reach_with_fixed_threads(const uint& n, const uint& z) {
	auto spw = z;
	map<Thread_State, list<Thread_State>> TTD; //TODO: record this at the begining
	queue<Global_State, list<Global_State>> W; /// worklist
	W.emplace(INITL_TS, n); /// start from the initial state with n threads
	set<Global_State> R; /// reachable global states
	while (!W.empty()) {
		Global_State tau = W.front();
		W.pop();
		const ushort &shared = tau.get_share();
		for (auto il = tau.get_locals().begin(); il != tau.get_locals().end();
				++il) {
			Thread_State src(shared, il->first);
			if (src != FINAL_TS) {
				auto ifind = TTD.find(src);
				if (ifind != TTD.end()) {
					for (auto idst = ifind->second.begin();
							idst != ifind->second.end(); ++idst) {
						Locals locals;
						if (Ufun::is_spawn_transition(src, *idst)) { /// if src +> dst
							if (spw > 0) {
								spw--;
								locals = this->update_counter(tau.get_locals(),
										idst->get_local());
							} else { /// if we already spawn z threads, we can't
								continue; /// spawn any more and have to skip it.
							}
						} else {
							locals = this->update_counter(tau.get_locals(),
									src.get_local(), idst->get_local());
						}

						//TODO Global_State _tau(idst->share, locals);
						if (R.emplace(idst->get_share(), locals).second) {
							/// recording _tau's predecessor tau for witness
							//TODO _tau.pi = std::make_shared<Global_State>(tau);
							W.emplace(idst->get_share(), locals);
						}
					}
				}
			} else { /// if src == final
				cout << "witness path: " << tau << endl;
				//TODO this->reproduce_witness_path(tau.pi);
				return true;
			}
		}
	}
	return false;
}

/**
 * @brief This procedure updates the counters of local states.
 * @param Z  : the part of local states
 * @param inc: local state whose counter is incremented
 * @return local states after updating counters
 */
Locals FWS::update_counter(const map<ushort, ushort> &Z, const ushort &inc) {
	auto _Z = Z;   /// local copy of Z

	auto iinc = _Z.find(inc);
	if (iinc != _Z.end()) {
		iinc->second++;
	} else {
		_Z[inc] = 1;
	}

	return _Z;
}

/**
 * @brief This procedure updates the counters of local states.
 * @param Z  : the part of local states
 * @param dec: local state whose counter is decremented
 * @param inc: local state whose counter is incremented
 * @return local states after updating counters
 * 		representing in counter abstraction
 */
Locals FWS::update_counter(const Locals &Z, const Local_State &dec,
		const Local_State &inc) {
	if (dec == inc) /// if dec == inc, do nothing
		return Z;

	auto _Z = Z;   /// local copy of Z

	auto idec = _Z.find(dec);
	if (idec != _Z.end()) {
		idec->second--;
		if (idec->second == 0)
			_Z.erase(idec);
	} else {
		throw ural_rt_err("FWS::update_counter: local state miss");
	}

	auto iinc = _Z.find(inc);
	if (iinc != _Z.end()) {
		iinc->second++;
	} else {
		_Z[inc] = 1;
	}

	return _Z;
}
} /* namespace SURA */
