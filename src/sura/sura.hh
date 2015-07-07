/*
 * sura.hh
 *
 *  Created on: Jun 25, 2015
 *      Author: lpzun
 */

#ifndef SURA_HH_
#define SURA_HH_

#include "fws/fws.hh"

#include "../util/utilities.hh"

namespace sura {

class Sura {
public:
	Sura();
	virtual ~Sura();

	bool symbolic_reachability_analysis(const string& filename, const string& initl_ts, const string& final_ts);

private:
	void parse_input_ttd(const string& filename);
	Thread_State parse_input_tss(const string& str_ts);

	bool reachability_as_logic_decision(const adj_list& TTD);
};

} /* namespace sura */

#endif /* SURA_HH_ */
