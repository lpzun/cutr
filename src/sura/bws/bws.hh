/**
 * @brief bws.hh
 *
 * @date  : Apr 23, 2016
 * @author: lpzun
 */

#ifndef SURA_BWS_BWS_HH_
#define SURA_BWS_BWS_HH_
#include "z3++.h"

#include "../../pwa/scc.hh"
#include "../ufun.hh"

using namespace z3;

namespace sura {

class BWS {
public:
	BWS();
	~BWS();

	bool concolic_BWS();

private:
	deque<thread_state> initl_ts;
	deque<thread_state> final_ts;
	adj_list reverse_TTD;
	map<shared_state, set<local_state>> expansion_L;

	bool standard_BWS();
	deque<syst_state> step(const syst_state& _tau);

	bool is_reached(const syst_state& tau);
	bool is_covered(const syst_state& tau1, const syst_state& tau2);
	bool is_minimal(const syst_state& tau, deque<syst_state>& R);

	bool is_connected();
};

} /* namespace sura */

#endif /* SURA_BWS_BWS_HH_ */
