/*
 * ettd.cc
 *
 *  Created on: Jun 21, 2015
 *      Author: lpzun
 */

#include "ettd.hh"

namespace SURA {

template<typename T>
Transition<T>::Transition(const T& src, const T& dst) :
		src(src), dst(dst), type(NORM) {
}

template<typename T>
Transition<T>::Transition(const T& src, const T& dst, const type_T& type) :
		src(src), dst(dst), type(type) {
}

template<typename T>
Transition<T>::~Transition() {
}

/// Global static pointer used to ensure a single instance of the class.
unique_ptr<ETTD> ETTD::m_instance = nullptr;

/**
 * @brief This function is called to create an instance of the class.
 * 		  Calling the constructor publicly is not allowed. The constructor
 * 		  is private and is only called by this instance function.
 *
 * @return
 */
ETTD& ETTD::instance() {
	if (!m_instance)
		m_instance.reset(std::unique_ptr<ETTD>(new ETTD(original_TTD)).release());
	return *m_instance;
}

ETTD::ETTD() {
	// TODO Auto-generated constructor stub
}

ETTD::ETTD(const adj_list& TTD) {
	this->build_ETTD(TTD);
}

ETTD::~ETTD() {
	// TODO Auto-generated destructor stub
}

}
/* namespace SURA */
