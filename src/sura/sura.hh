/**
 * @brief sura.hh
 *
 * @date: Jun 25, 2015
 * @author: Peizun Liu
 */

#ifndef SURA_HH_
#define SURA_HH_

#include "fws/fws.hh"

#include "../pwa/scc.hh"
#include "../util/utilities.hh"

namespace sura {

class Sura {
public:
	Sura();
	virtual ~Sura();

	bool symbolic_reachability_analysis(const string& filename,
			const string& initl_ts, const string& final_ts);

private:
	vector<inout> parse_input_ttd(const string& filename);
	Thread_State parse_input_tss(const string& str_ts);

	bool reachability_as_logic_decision(const adj_list& TTD,
			const vector<inout>& s_in_out);

	bool path_wise_analysis(const shared_ptr<GSCC>& p_gscc);
};

} /* namespace sura */

#endif /* SURA_HH_ */
