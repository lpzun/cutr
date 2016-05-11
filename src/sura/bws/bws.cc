/**
 * @brief bws.cc
 *
 * @date  : Apr 23, 2016
 * @author: lpzun
 */

#include "bws.hh"

namespace sura {

BWS::BWS() {

}

BWS::~BWS() {

}

/**
 * @brief the concolic backward search
 * @return
 */
bool BWS::concolic_BWS() {
    if (Refs::OPT_PRINT_ADJ || Refs::OPT_PRINT_ALL) {
        cout << "The reverse TTD:\n";
        for (auto isrc = Refs::reversee_TTD.begin();
                isrc != Refs::reversee_TTD.end(); ++isrc) {
            for (auto idst = isrc->second.begin(); idst != isrc->second.end();
                    ++idst) {
                cout << Refs::mapping_TS[isrc->first];
                if (ETTD::spaw_R[*idst][isrc->first])
                    cout << " +> ";
                 else
                    cout << " -> ";
                cout << Refs::mapping_TS[*idst] << "\n";
            }
        }
        cout << endl;
    }
    return this->standard_BWS();
}

/**
 * @brief the standard backward search
 * @return
 */
bool BWS::standard_BWS() {
    antichain worklist;
    antichain explored;
    worklist.emplace_back(Refs::FINAL_TS);
    while (!worklist.empty()) {
        const auto _tau = worklist.front();
        worklist.pop_front();

        /// step 1: if there exists t in <explored> such that
        ///         t <= _tau, then discard _tau
        if (this->is_minimal(_tau, explored)) {
            const auto& images = this->step(_tau);
            /// step 2: insert all preimages to worklist
            for (const auto& tau : images) {
                if (this->is_initial(tau))
                    return true;
                worklist.emplace_back(tau);
            }
            this->minimize(_tau, explored);
            explored.emplace_back(_tau);
        }
    }
    return false;
}

/**
 * @brief step function
 * @param _tau
 * @return the cover preimages
 */
deque<syst_state> BWS::step(const syst_state& _tau) {
    auto icand = Refs::candidate_L.find(_tau.get_share());
    if (icand == Refs::candidate_L.end())
        return deque<syst_state>();
    deque<syst_state> images;
    for (const auto& local : icand->second) {
        /// source TS of the reverse edge
        const thread_state curr(_tau.get_share(), local);
        auto icurr = Refs::activee_TS.find(curr);
        if (icurr != Refs::activee_TS.end()) {
            auto ifind = Refs::reversee_TTD.find(icurr->second);
            if (ifind != Refs::reversee_TTD.end()) {
                for (const auto& iprev : ifind->second) {
                    const auto& prev = Refs::mapping_TS[iprev];
                    if (ETTD::spaw_R[iprev][icurr->second]) {
//                        cout << prev << " +> " << curr << endl;
                        bool is_updated = true;
                        const auto& Z = Ufun::update_counter(_tau.get_locals(),
                                local, prev.get_local(), is_updated);
                        if (is_updated)
                            images.emplace_back(prev.get_share(), Z);
                    } else {
                        const auto& Z = Ufun::update_counter(_tau.get_locals(),
                                local, prev.get_local());
                        images.emplace_back(prev.get_share(), Z);
                    }
                }
            } /// reverse TTD
        }
    }
//    cout << __func__ << "\n";
//    for (const auto& t : images)
//        cout << t << "\n";
    return images;
}

/**
 * @brief determine if tau is in upward closure of INITL_TS
 * @param tau
 * @return bool
 */
bool BWS::is_initial(const syst_state& tau) {
    if (tau.get_share() == Refs::INITL_TS.get_share()) {
        if (tau.get_locals().size() == 1
                && tau.get_locals().begin()->first
                        == Refs::INITL_TS.get_local())
            return true;
    }
    return false;
}

/**
 * @brief to determine if s is a minimal state w.r.t. R
 * @param s
 * @param R
 * @return
 */
bool BWS::is_minimal(const syst_state& tau, antichain& R) {
    for (const auto& t : R) {
        if (is_covered(t, tau))
            return false;
    }
    return true;
}

void BWS::minimize(const syst_state& tau, antichain& R) {
    for (auto im = R.begin(); im != R.end();) {
        if (is_covered(tau, *im))
            im = R.erase(im);
        else
            ++im;
    }
}
/**
 * @brief
 * @param tau1
 * @param tau2
 * @return
 */
bool BWS::is_covered(const syst_state& s1, const syst_state& s2) {
    if (s1.get_share() == s2.get_share()
            && s1.get_locals().size() <= s2.get_locals().size()) {
        auto is1 = s1.get_locals().cbegin();
        auto is2 = s2.get_locals().cbegin();
        while (is1 != s1.get_locals().cend()) {
            /// check if is2 reaches to the end
            if (is2 == s2.get_locals().cend())
                return false;
            /// compare the map's contents
            if (is1->first == is2->first) {
                if (is1->second <= is2->second)
                    ++is1, ++is2;
                else
                    return false;
            } else if (is1->first > is2->first) {
                ++is2;
            } else if (is1->first < is2->first) {
                return false;
            }
        }
        return true;
    }
    return false;
}

} /* namespace sura */
