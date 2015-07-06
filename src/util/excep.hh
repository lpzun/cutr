/**
 * @name excep.hh
 *
 * @date Jan 12, 2015
 * @author Peizun Liu
 */

#ifndef EXCEP_HH_
#define EXCEP_HH_

#include <stdexcept>
#include <exception>
#include <string>

using std::runtime_error;
using std::exception;
using std::string;

class ural_rt_err: public runtime_error {
public:
	ural_rt_err() :
			runtime_error("") {
	}
	ural_rt_err(const std::string& msg) :
			runtime_error(msg) {
	}
};


class sura_exception: public exception {
public:
	string message;
	short code;
	inline sura_exception() :
			message(""), code(-1) {

	}
	inline sura_exception(const string& message, const short& code = -1) :
			message(message), code(code) {
	}    // must provide some message

	virtual const char* what() const throw () {
		return message.c_str();
	}
};

#endif /* EXCEP_HH_ */
