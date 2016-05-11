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
    bool standard_BWS();
    deque<syst_state> step(const syst_state& _tau);

    bool is_initial(const syst_state& tau);
    bool is_covered(const syst_state& s1, const syst_state& s2);
    bool is_minimal(const syst_state& tau, antichain& R);

    void minimize(const syst_state& tau, antichain& R);

    bool is_connected();
};

} /* namespace sura */

#endif /* SURA_BWS_BWS_HH_ */
