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

class cutr_rt_err: public runtime_error {
public:
	cutr_rt_err() :
			runtime_error("") {
	}
	cutr_rt_err(const std::string& msg) :
			runtime_error(msg) {
	}
};


class cutr_exception: public exception {
public:
	string message;
	short code;
	inline cutr_exception() :
			message(""), code(-1) {

	}

	inline cutr_exception(const string& message, const short& code = -1) :
			message(message), code(code) {
	}

	virtual const char* what() const throw () {
		return message.c_str();
	}
};

#endif /* EXCEP_HH_ */
