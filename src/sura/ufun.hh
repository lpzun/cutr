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
	virtual ~Ufun();

	static Locals update_counter(const Locals &Z, const Local_State &dec,
			const Local_State &inc);

	static delta compute_delta(const deque<edge>& ifx);
	static delta compute_delta(const edge& ifx);

	static deque<edge> extract_trans_enter_to_exit(const deque<edge>& E,
			const vertex& en, const vertex& ex);

private:
	static void update_counter(delta &delta, const edge &e);
};

} /* namespace sura */

#endif /* UFUN_HH_ */
