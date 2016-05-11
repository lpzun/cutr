/**
 * ufun.hh
 *
 *  Created on: Jul 12, 2015
 *      Author: lpzun
 */

#ifndef UFUN_HH_
#define UFUN_HH_

#include "../util/state.hh"
#include "../pwa/scc.hh"

namespace sura {

typedef unordered_map<ushort, short> delta; /// Aliasing unordered_map<ushort, short> as delta
typedef unordered_map<ushort, ushort> bound; /// Aliasing unordered_map<ushort, ushort> as bound

class Ufun {
public:
    Ufun();
    ~Ufun();

    /// used for BWS or FWS
    static ca_locals update_counter(const ca_locals &Z, const local_state &dec,
            const local_state &inc);
    static ca_locals update_counter(const ca_locals &Z, const local_state &dec,
            const local_state &inc, bool& is_updated);
    static ca_locals increment_counter(const ca_locals &Z, const ushort &inc);
    static ca_locals decrement_counter(const ca_locals &Z, const ushort &dec);

    static delta compute_delta(const deque<edge>& ifx);
    static delta compute_delta(const edge& ifx);

    static deque<edge> extract_trans_enter_to_exit(const deque<edge>& E,
            const vertex& en, const vertex& ex);

private:
    static void update_counter(delta &delta, const edge &e);
};

} /* namespace sura */

#endif /* UFUN_HH_ */
