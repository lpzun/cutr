/*
 * sura.hh
 *
 *  Created on: Jun 25, 2015
 *      Author: lpzun
 */

#ifndef SURA_HH_
#define SURA_HH_

//#include <fstream>

#include "fws/fws.hh"

#include "../util/utilities.hh"

//using std::ifstream;
//using std::ofstream;

namespace sura {

class Sura {
public:
	Sura();
	virtual ~Sura();

	bool symbolic_reachability_analysis(const string& infile, const string& initl_ts, const string& final_ts);

private:
	void parse_input_ttd(const string& ttd);
	void parse_input_tss(const string& ts);
};

} /* namespace sura */

#endif /* SURA_HH_ */
