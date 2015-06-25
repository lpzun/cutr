/*
 * state.hh
 *
 *  Created on: Jun 21, 2015
 *      Author: lpzun
 */

#ifndef STATE_HH_
#define STATE_HH_

#include "heads.hh"

namespace sura {

/// define local state
typedef unsigned short Local_State;
/// size of local states
typedef unsigned short size_l;

/// define shared state
typedef unsigned short Shared_State;
/// size of shared states
typedef unsigned short size_s;

/// define the counter of thread state
typedef unsigned short size_p;

/// class thread state
class Thread_State {
public:
	static size_s S; /// the size of shared state
	static size_l L; /// the size of local  state

	Shared_State share;
	Local_State local;

	inline Thread_State();
	inline Thread_State(const Thread_State& t);
	inline Thread_State(const Shared_State& share, const Local_State& local);
	virtual ~Thread_State() {
	}

	ostream& to_stream(ostream& out = cout) const;
};

/**
 * @brief default constructor
 */
inline Thread_State::Thread_State() :
		share(0), local(0) {
}

/**
 * @brief constructor with thread state
 * @param t
 */
inline Thread_State::Thread_State(const Thread_State& t) :
		share(t.share), local(t.local) {

}

/**
 * @brief constructor with shared state and local state
 * @param share: shared state
 * @param local: local  state
 */
inline Thread_State::Thread_State(const Shared_State& share, const Local_State& local) {
	assert(share < S && local < L);
	this->share = share;
	this->local = local;
}

/**
 * @brief print thread state
 * @param out
 * @return ostream
 */
inline ostream& Thread_State::to_stream(ostream& out) const {
	out << "(" << share << "|" << local << ")";
	return out;
}

/**
 * @brief overloading operator <<: print thread state
 * @param out
 * @param ts
 * @return ostream
 */
inline ostream& operator <<(ostream& out, const Thread_State& t) {
	return t.to_stream(out);
}

/// class global state
typedef unordered_map<Local_State, size_p> Locals;

class Global_State {
public:
	Shared_State share;
	Locals locals;
	shared_ptr<const Global_State> pi;

	inline Global_State();
	inline Global_State(const Thread_State& t);
	inline Global_State(const Shared_State& share, const Locals& locals);
	inline Global_State(const Shared_State& share, const Locals& locals, shared_ptr<const Global_State> pi);
	virtual ~Global_State() {
	}
};

/**
 * @brief default constructor: initialize
 *        share  = 0
 *        locals = empty map
 */
inline Global_State::Global_State() :
		share(0), locals(Locals()), pi(nullptr) {
}

/**
 * @brief constructor with a thread state
 * @param t
 */
inline Global_State::Global_State(const Thread_State& t) :
		share(t.share), pi(nullptr) {
	locals[t.local] = 1;
}

/**
 * @brief constructor with a shared state and local states
 * @param share : shared state
 * @param locals: local states represented in counter abstraction form
 */
inline Global_State::Global_State(const Shared_State& share, const Locals& locals) :
		share(share), locals(locals), pi(nullptr) {
}

/**
 * @brief constructor with a shared state, local states and pi
 * @param share : shared state
 * @param locals: local states represented in counter abstraction form
 * @param pi	: the father of current global state
 */
inline Global_State::Global_State(const Shared_State& share, const Locals& locals,
		shared_ptr<const Global_State> pi) :
		share(share), locals(locals), pi(pi) {
}

} /* namespace SURA */

#endif /* STATE_HH_ */
