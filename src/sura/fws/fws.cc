/**
 * @brief fws.cc
 *
 * @date  : Jun 25, 2015
 * @author: Peizun Liu
 */

#include "fws.hh"

namespace sura {

////////////////////////////////////////////////
////  static class variables defined from here
////
////////////////////////////////////////////////

/// All expressions, func_decl, etc., appearing in the class must be
/// defined in same context; otherwise, segmentation fault happens
context FWS::ctx;

/// global variables used in the class to build the path constraint
string FWS::n_affix = "n"; /// prefix for variables of local state counters

string FWS::k_affix = "k"; /// prefix for loop iteration variables
string FWS::x_affix = "x"; /// prefix for marking equation variables

/// counter variable for initial local state
expr FWS::n_0 = ctx.int_const((n_affix + std::to_string(Refs::INITL_TS.get_local())).c_str());

uint FWS::max_n = 0;  /// maximal number of initial   threads in max path
uint FWS::max_z = 0;  /// maximal number of spawn transitions in max path
uint FWS::con_z = 0;  /// the sum of constant spawn transitions in max path

/// expression of summarizing fired spawns in max path
expr FWS::sum_z = ctx.int_val(max_z);

////////////////////////////////////////////////
////  function members defined from here
////
////////////////////////////////////////////////
/**
 * @brief FWS constructor
 * @param size_P: the number of SCC quotient paths
 * @param p_gscc: the pointer to SCC quotient graph
 */
FWS::FWS(const size_t& size_P, const shared_ptr<GSCC>& p_gscc) :
		k_index(0), x_index(Transition::ID), p_gscc(p_gscc), solver_P(), sat_P(vector<bool>(size_P, false)), spaw_vars(
				set<id_tran>()) {
	this->solver_P.reserve(size_P);
}

/**
 * @brief default deconstructor
 */
FWS::~FWS() {
}

/**
 * @brief forward-based symbolic reachability analysis
 * @param paths
 * 			NOTE: a path is a sequence of id_scc
 * @return bool
 * 		true : if final is reachable
 * 		false: otherwise
 */
bool FWS::fws_as_logic_decision(const vector<_path>& paths) {
	bool is_exists_sat_path = false;
	for (auto ipath = paths.begin(); ipath != paths.end(); ++ipath) {
		switch (this->quotient_path_reachability(*ipath)) {
		case result::reach:
			return true;
		case result::unknown:
			if (!is_exists_sat_path)
				is_exists_sat_path = true;
			break;
		case result::unreach:
			break;
		}
	}

	if (!is_exists_sat_path)
		return false;
	return this->solicit_for_CEGAR();
}

/**
 * @brief splice a SCC quotient path as a set of real path
 * @param P
 * @return
 */
result FWS::quotient_path_reachability(const _path& P) {
#ifndef NDEBUG
	cout << __func__ << endl;
	for (auto iscc = P.begin(); std::next(iscc) != P.end(); ++iscc) {
		cout << " => " << *iscc << " by ";
		if (p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(iscc))] != nullptr)
		for (const auto& t : *(p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(
										iscc))]))
		cout << t << " ";
	}
	cout << endl;
#endif

	deque<deque<size_t>> las_level;                /// last       level
	deque<deque<size_t>> cur_level;               /// current level
	las_level.emplace_back(deque<size_t>());   /// initialize last level

	/// run BFS to iterate over and store all of paths that belong to the
	/// same SCC quotient path: note that we just store the indices
	for (auto iscc = P.begin(); std::next(iscc) != P.end(); ++iscc) {
		const auto p_outgoing = p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(iscc))];
		for (size_t i = 0; i < p_outgoing->size(); ++i) {
			for (auto p = las_level.begin(); p != las_level.end(); ++p) {
				auto np = *p;
				np.emplace_back(i);
				cur_level.emplace_back(np);
			}
		}
		las_level.clear();
		cur_level.swap(las_level);
	}

#ifndef NDEBUG
	cout << "------------path size------------------------" << P.size() << "\n";
	cout << __func__ << " Iterator: \n";
	for (auto ip = las_level.begin(); ip != las_level.end(); ++ip) {
//		for (auto ie = ip->begin(); ie != ip->end(); ++ie) {
//			cout << *ie << "\n";
//		}
		cout<<"size "<<ip->size()<<"\n";
	}
#endif

	/// iterate over all of the real paths that belong to the same quotient
	/// path P
	result is_exists_sat_path = result::unreach;
	for (auto ip = las_level.begin(); ip != las_level.end(); ++ip) {
		this->solver_P.emplace_back(nullptr); /// a pointer to a solver for P
		const auto index = solver_P.size() - 1;  /// get the index for current P
		this->sat_P[index] = false;
		const auto& phi = this->path_summary(P, *ip);

		switch (this->path_reachability(phi, index)) {
		case result::reach:
			return result::reach;
		case result::unreach:
			this->solver_P[index] = nullptr;
			break;
		case result::unknown:
			if (is_exists_sat_path == result::unreach)
				is_exists_sat_path = result::unknown;
			this->sat_P[index] = true;
			break;
		}
	}
	return is_exists_sat_path;
}

/**
 * @brief encoding a path as a Presburger formula: forward version
 * @param phi
 * @param id_P
 * @return  bool
 * 		true : if the encoding formula is satisfiable
 * 		false: otherwise
 */
result FWS::path_reachability(const vec_expr& phi, const ushort& id_P) {
	if (solver_P[id_P] == nullptr)
		solver_P[id_P] = std::make_shared<solver>(
				(tactic(ctx, "simplify") & tactic(ctx, "qe") & tactic(ctx, "smt")).mk_solver());

	/// add k_i >= 0
	for (auto idx = 0; idx < k_index; ++idx)
		solver_P[id_P]->add(ctx.int_const((k_affix + std::to_string(idx)).c_str()) >= 0);

	/// add x_i >= 0
	for (size_t idx = 0; idx < x_index; ++idx)
		solver_P[id_P]->add(ctx.int_const((x_affix + std::to_string(idx)).c_str()) >= 0);

	/// add constraints
	for (auto iphi = phi.begin(); iphi != phi.end(); ++iphi)
		solver_P[id_P]->add(*iphi);

#ifndef NDEBUG
	cout << __func__ << " " << phi.size() << "\n";
	unsigned i = 0;
	for (auto iphi = phi.begin(); iphi != phi.end(); ++iphi, ++i) {
		if (i < Thread_State::L)
		cout << "l" << i << *iphi << "\n";
		else
		cout << "s" << i - Thread_State::L << *iphi << "\n";
	}
#endif

	return this->check_sat_via_smt_solver(solver_P[id_P]);
}

/**
 * @brief encoding a SCC quotient path as a Presburger formula: forward version
 * @param P
 * @param permu
 * @return
 */
vec_expr FWS::path_summary(const _path& P, const deque<size_t>& permu) {
	/// pfx: The vector of expressions: store prefix formula
	/// 	 Range [0, l) store constraints for local states
	///		 Range [l, l+s) store constraints for shared states
	vec_expr pfx(Thread_State::L + Thread_State::S, ctx.int_val(0));
	pfx[Refs::INITL_TS.get_local()] = n_0;

	/// phi: The vector of expressions: store path summary constraints
	/// 	 Range [0, l) store constraints for local states
	///		 Range [l, l+s) store constraints for shared states
	vec_expr phi(Thread_State::L + Thread_State::S, ctx.bool_val(true));
	phi[Refs::INITL_TS.get_local()] = n_0 > 1; /// constraint n_0 > 1

	/// reset spawn expression
	sum_z = ctx.int_val(0);
	con_z = 0;
	this->k_index = 0;

#ifndef NDEBUG
	cout << __func__ << "Iterator: \n";
	for (auto ie = permu.begin(); ie != permu.end(); ++ie)
		cout << *ie << "\n";
	cout << endl;
#endif

	deque<edge> infix;
	/// transitions from last SCC to current SCC: incoming
	shared_ptr<deque<edge>> p_incoming(nullptr);
	auto ipos = permu.begin();
	for (auto iscc = P.begin(); std::next(iscc) != P.end(); ++iscc, ++ipos) { /// iterate over P
		/// pointer to current SCC
		auto p_scc = p_gscc->get_sccs()[*iscc];
		/// transitions from current SCC to the next SCC: outgoing
		auto p_outgoing = p_gscc->get_trans_btwn_sccs()[*iscc][*(std::next(iscc))];

		if (p_scc->is_trivial() == false) { /// if current SCC is non-trivial
			/// CASE 1: assemble to <pfx> and <phi> the arithmetic for infix
			this->assemble(pfx, phi, Ufun::compute_delta(infix));
			infix.clear(); /// clear all edges in the last infix

			/// define entry point
			auto en(INITL_V);
			if (p_incoming != nullptr)
				en = (p_incoming->begin() + *(std::prev(ipos)))->get_dst();

			/// define exit  point
			auto ex = (p_outgoing->begin() + *(ipos))->get_src();

			if (p_scc->is_nested() == false) { /// simple loop
				/// extract the "half" cycle from entry point to exit point
				auto en_to_ex = Ufun::extract_trans_enter_to_exit(p_scc->get_E(), en, ex);
				infix.insert(infix.end(), en_to_ex.begin(), en_to_ex.end());

				/// CASE 2: assemble to <pfx> and <phi> the arithmetic for
				///         simple loop
				this->assemble(pfx, phi, Ufun::compute_delta(p_scc->get_E()),
						ctx.int_const((k_affix + std::to_string(k_index++)).c_str()));
			} else { /// nested loop
				/// CASE 3: assemble to <pfx> and <phi> the arithmetic for
				///         loop nests
				/// compute thread state equation: C_S and C_L constraints
				auto is_append = this->append_marking_equation(pfx, *p_scc);
				/// assemble C_L constraints
				this->assemble(pfx, phi, is_append);
				/// assemble C_S constraints
				this->assemble(pfx, phi, Refs::mapping_TS[en].get_share(), Refs::mapping_TS[ex].get_share(), is_append);
			}
		}
		/// append the ipos^th transition between current SCC and its successor
		infix.emplace_back((p_outgoing->begin() + *(ipos))->get_src(), (p_outgoing->begin() + *(ipos))->get_dst());
		p_incoming = std::move(p_outgoing);
	}
	if (infix.size() > 0) {
		/// assemble the suffix: the last straight segment
		auto ifx_d = Ufun::compute_delta(infix);
		this->assemble(pfx, phi, ifx_d);

		/// n_f >= 1
		phi[Refs::FINAL_TS.get_local()] = phi[Refs::FINAL_TS.get_local()]
				&& (pfx[Refs::FINAL_TS.get_local()] + ifx_d[Refs::FINAL_TS.get_local()] >= 1);
	}

	/// build spawn expression to summarize spawn transitions
	sum_z = sum_z + ctx.int_val(con_z);
	return phi;
}

/**
 * @brief assemble infix formula to prefix arithmetic <pfx> and <phi>
 * 			NOTE: this method computes the sum of constant spawns
 * @param pfx  : formula to summarize thread counters for each local state
 * 				 in the prefix of current quotient path
 * @param phi  : constraint for each local state in the prefix  of current
 * 				 quotient path
 * @param delta: the delta for prefix path
 */
void FWS::assemble(vec_expr &pfx, vec_expr &phi, const delta &delta) {
	auto sum = 0;
	for (auto id = delta.begin(); id != delta.end(); id++) {
		phi[id->first] = phi[id->first] && (pfx[id->first] + id->second >= 0);
		pfx[id->first] = pfx[id->first] + id->second;
		sum += id->second;
	}
	if (sum > 0)
		con_z = con_z + sum;
}

/**
 * @brief assemble to <pfx> and <phi> the arithmetic for simple loop
 * 			NOTE: this method computes the sum of constant spawns
 * @param pfx  : formula to summarize thread counters for each local state
 * 				 in the prefix of current quotient path
 * @param phi  : constraint for each local state in the prefix  of current
 * 				 quotient path
 * @param delta: the delta for prefix path
 * @param k    : the simple loop iterator kappa
 */
void FWS::assemble(vec_expr &pfx, vec_expr &phi, const delta &delta, const expr &k) {
/// the intermediate variable used in <forall> quantifier
	expr i = ctx.int_const("i");
	expr r = 0 <= i && i <= k;
	auto sum = 0;
	for (auto id = delta.begin(); id != delta.end(); id++) {
		expr f = implies(r, pfx[id->first] + id->second * i >= 0);
		phi[id->first] = phi[id->first] && forall(i, f);
		pfx[id->first] = pfx[id->first] + id->second * k;
		sum += id->second;
	}
	if (sum > 0)
		sum_z = sum_z + sum * k;
}

/**
 * @brief assemble to <pfx> and <phi> the marking equation for loop nests:
 * 		  assemble local constraints to phi
 * @param pfx  : formula to summarize thread counters for each local state
 * 				 in the prefix of current quotient path
 * @param phi  : constraint for each local state in the prefix  of current
 * 				 quotient path
 * @param is_append: mark whether the entries of C_L/C_S are updated or not
 */
void FWS::assemble(const vec_expr &pfx, vec_expr &phi, const vector<bool> &is_append) {
	for (auto i = 0; i < Thread_State::L; ++i)
		if (is_append[i])
			phi[i] = phi[i] && pfx[i] >= 0;
}

/**
 *
 * @brief assemble to <pfx> and <phi> the marking equation for loop nests:
 * 		  assemble shared constraints to phi
 * @param pfx   : formula to summarize thread counters for each local state
 * 				 in the prefix of current quotient path
 * @param phi   : constraint for each local state in the prefix  of current
 * 				 quotient path
 * @param s_entr: shared state of entry point
 * @param s_exit: shared state of exit  point
 * @param is_append: mark whether the entries of C_L/C_S are updated or not
 */
void FWS::assemble(const vec_expr &pfx, vec_expr &phi, const Shared_State &s_entr, const Shared_State &s_exit,
		const vector<bool> &is_append) {
	for (auto i = Thread_State::L; i < Thread_State::L + Thread_State::S; ++i)
		if (is_append[i]) {
			if (s_entr != s_exit && i == Thread_State::S + s_entr)
				phi[i] = phi[i] && pfx[i] == -1;
			else if (s_entr != s_exit && i == Thread_State::S + s_exit)
				phi[i] = phi[i] && pfx[i] == 1;
			else
				phi[i] = phi[i] && pfx[i] == 0;
		}
}

/**
 * @brief append marking equations of a SCC to the pre SCC formula
 * @param pfx: a vector of formulae:
 * 		  we use it to return formulae to which after appending marking equation
 * @param scc: loop nests
 * @return vector<bool>
 * 		  to mark to which is appended marking equations
 */
vector<bool> FWS::append_marking_equation(vec_expr &pfx, const SCC &scc) {
	vector<bool> is_append(Thread_State::L + Thread_State::S, false);

	/// reset prefix for shared constraints
	for (auto i = Thread_State::L; i < Thread_State::L + Thread_State::S; ++i)
		pfx[i] = ctx.int_val(0);

	const auto& transitions = scc.get_E();
	for (auto itran = transitions.begin(); itran != transitions.end(); ++itran) {
		/// declare C_L variables for marking equation
		expr x = ctx.int_const((x_affix + std::to_string(itran->get_id())).c_str());
		if (ETTD::spaw_R[itran->get_src()][itran->get_dst()]) {
			spaw_vars.emplace(itran->get_id());
		}

		/// define C_L constraints
		auto delta = Ufun::compute_delta(*itran);
		for (auto il = delta.begin(); il != delta.end(); ++il) {
			pfx[il->first] = pfx[il->first] + x * il->second;
			if (!is_append[il->first])
				is_append[il->first] = true;
		}

		/// define C_S constraints
		const auto& s_src = Refs::mapping_TS[itran->get_src()].get_share(); /// shared state of      source TS
		const auto& s_dst = Refs::mapping_TS[itran->get_dst()].get_share(); /// shared state of destination TS
		if (s_src != s_dst) {
			pfx[Thread_State::L + s_src] = pfx[Thread_State::L + s_src] - x; /// incoming - x
			if (!is_append[Thread_State::L + s_src])
				is_append[Thread_State::L + s_src] = true;

			pfx[Thread_State::L + s_dst] = pfx[Thread_State::L + s_dst] + x; /// outgoing + x
			if (!is_append[Thread_State::L + s_dst])
				is_append[Thread_State::L + s_dst] = true;
		}
	}
	return is_append;
}

/**
 * @brief checking if a constraint is satisfiable or not
 * @param s: a SMT solver
 * @return bool
 * 		true : if s is satisfiable
 * 		false: otherwise
 */
result FWS::check_sat_via_smt_solver(shared_ptr<solver>& s) {
	switch (s->check()) {
	case sat:   /// if   sat
		if (this->parse_sat_solution(s->get_model())) /// if max_n or max_z is updated
			if (this->check_reach_with_fixed_threads(max_n, ++max_z)) /// if find out a witness
				return result::reach;
		return result::unknown;
	case unsat: /// if unsat
		return result::unreach;
	case unknown:
		throw ural_rt_err("smt solver returns unknow!");
	}
}

/**
 * @brief parse the sat solution and see if max_n or max_z is updated
 * @param m: the solution returned from z3 SMT solver
 * @return bool
 * 		   true : if max_n or max_z is updated
 * 		   false: otherwise
 */
bool FWS::parse_sat_solution(const model& m) {
	uint n = 0;
	for (size_t i = 0; i < m.size(); i++) {
		func_decl v = m[i];
		assert(v.arity() == 0); /// check if contains only constants
		if (v.name() == n_0.decl().name())
			if (Z3_get_numeral_uint(ctx, m.get_const_interp(v), &n))
				break;
	}
	auto z = get_z3_const_uint(m.eval(sum_z));
	if (max_n < n || max_z < z) {
		if (max_n < n) /// update max_n if necessary
			max_n = n;
		if (max_z < z) /// update max_z if necessary
			max_z = z;
		return true;  /// max_n or max_z is updated
	}
	return false;
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
	return spaw_vars.find(std::stoi(v.substr(x_affix.size()))) != spaw_vars.end();
}

/**
 * @brief solicit for CEGAR to refine pathwise encoding
 * 		  TODO: don't have to use recursion
 * @return bool
 * 		true : if we find a witness path,
 * 		false: if all of paths finally are unsatisfiable.
 */
bool FWS::solicit_for_CEGAR() {
	bool is_exists_sat_path = true;
	while (is_exists_sat_path) {
		for (size_t idx = 0; idx < sat_P.size(); ++idx) {
			if (sat_P[idx] == true) {
				if (!is_exists_sat_path)
					is_exists_sat_path = true;

				/// add increment constraints
				solver_P[idx]->add(n_0 > ctx.int_val(max_n));
				if (max_z > 0 && !sum_z.is_int())
					solver_P[idx]->add(sum_z > ctx.int_val(max_z));

				/// apply incremental solving
				switch (check_sat_via_smt_solver(solver_P[idx])) {
				case result::reach:
					return true;
				case result::unreach:
					sat_P[idx] = false;
					solver_P[idx] = nullptr;
					is_exists_sat_path = false;
					break;
				case result::unknown:
					break;
				}
			}
		}

		if (is_exists_sat_path == false) /// exit while loop
			break;
	}
	return false;
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
//	cout << "I am here ........................." << n << " " << z << endl;
	auto spw = z;
	queue<Global_State, deque<Global_State>> W; /// worklist
	W.emplace(Refs::INITL_TS, n); /// start from the initial state with n threads
	set<Global_State> R; /// reachable global states
	while (!W.empty()) {
		Global_State tau = W.front();
		W.pop();
		const ushort &shared = tau.get_share();
		for (auto il = tau.get_locals().begin(); il != tau.get_locals().end(); ++il) {
			const Thread_State src(shared, il->first); /// source TS
			if (src != Refs::FINAL_TS) {
				auto isrc = Refs::activee_TS.find(src); /// get the id of source TS
				if (isrc != Refs::activee_TS.end()) {
					auto ifind = Refs::original_TTD.find(isrc->second);
					if (ifind != Refs::original_TTD.end()) {
						for (auto idst = ifind->second.begin(); idst != ifind->second.end(); ++idst) {
							const auto &dst = Refs::mapping_TS[*idst]; /// destination TS
							Locals locals;
							if (ETTD::spaw_R[isrc->second][*idst]) { /// if src +> dst
								if (spw > 0) {
									spw--;
									locals = this->update_counter(tau.get_locals(), dst.get_local());
								} else { /// if we already spawn z threads, we can't
									continue; /// spawn any more and have to skip it.
								}
							} else {
								locals = this->update_counter(tau.get_locals(), src.get_local(), dst.get_local());
							}

							if (R.emplace(dst.get_share(), locals).second) {
								/// recording _tau's predecessor tau for witness
								W.emplace(dst.get_share(), locals);
							}
						}
					}
				}
			} else { /// if src == final
				cout << "witness path: " << tau << endl;
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
Locals FWS::update_counter(const Locals &Z, const Local_State &dec, const Local_State &inc) {
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

/**
 * @brief
 * @param pi
 * @param R
 */
void FWS::reproduce_witness_path(const shared_ptr<Global_State>& pi, const set<Global_State> &R) {
	cout << *pi << endl; ///
	if (pi == nullptr || this->is_initial_state(*pi)) {
		cout << *pi << endl;
		return;
	} else {
		// this->reproduce_witness_path(pi->get_pi(), R);
	}
}

/**
 * @brief determine if tau is a global state
 * @param tau
 * @return bool
 * 		true : if tau is a global state
 * 		false: otherwise
 */
bool FWS::is_initial_state(const Global_State &tau) {
	if (tau.get_share() == Refs::INITL_TS.get_share()) {
		if (tau.get_locals().size() == 1) {
			if (tau.get_locals().begin()->first == Refs::INITL_TS.get_local() && tau.get_locals().begin()->second > 0)
				return true;
		}
	}
	return false;
}
} /* namespace SURA */
