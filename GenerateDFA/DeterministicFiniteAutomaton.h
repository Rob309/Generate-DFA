#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <regex>
#include <sstream>
#include "NedeterministicFiniteAutomaton.h"
#include <functional>
using namespace std;
class DeterministicFiniteAutomaton {
public:
    DeterministicFiniteAutomaton(const set<string>& q, const set<char>& sigma,
        const map<string, map<char, string>>& delta, const string& q0,
        const set<string>& f)
        : states(q), alphabet(sigma), transitions(delta), start_state(q0), accept_states(f) {}

    bool VerifyAutomation(string& error_message)
    {
        if (states.empty()) {
            error_message = "Q is empty, DeterministicFiniteAutomaton is not valid\n";
            return false;
        }
        if (alphabet.empty()) {
            error_message = "E is empty, DeterministicFiniteAutomaton is not valid\n";
            return false;
        }
        if (states.find(start_state) == states.end()) {
            error_message = "Start state " + start_state + " is not included in Q, DeterministicFiniteAutomaton is not valid\n";
            return false;
        }
        if (!accept_states.empty()) {
            for (auto& it : accept_states) {
                if (states.find(it) == states.end()) {
                    error_message = "Final state " + it + " does not belong in Q, DeterministicFiniteAutomaton is not valid\n";
                    return false;
                }
            }
        }

        for (const auto& [state, transitions_for_state] : transitions) {
            if (states.find(state) == states.end()) {
                error_message = "State " + state + " is not defined in Q, DeterministicFiniteAutomaton is not valid\n";
                return false;
            }
            for (const auto& [symbol, next_state] : transitions_for_state) {
                if (alphabet.find(symbol) == alphabet.end()) {
                    error_message = "Symbol " + string(1, symbol) + " is not in the alphabet E, DeterministicFiniteAutomaton is not valid\n";
                    return false;
                }
                if (states.find(next_state) == states.end()) {
                    error_message = "Next state " + next_state + " is not defined in Q, DeterministicFiniteAutomaton is not valid\n";
                    return false;
                }
            }
        }

        return true;
    }

    friend ostream& operator<<(ostream &os, DeterministicFiniteAutomaton dfa) {
        os << "States (Q): ";
        for (const string& state : dfa.states) os << state << " ";
        os << "\n Alphabet (E): ";
        for (char symbol : dfa.alphabet) os << symbol << " ";
        os << "\n Start State (q0): " << dfa.start_state;

        os << "\n Accept States (F): ";
        for (const string& state : dfa.accept_states) os << state << " ";
        os << "\n Transitions (6):\n";
        for (const auto& [state, transitions_for_state] : dfa.transitions) {
            for (const auto& [symbol, next_state] : transitions_for_state) {
                os << "  (" << state << ", " << symbol << ") -> " << next_state << "\n";
            }
        }
        return os;
    }

    void PrintAutomaton() const {
        cout << "States (Q): ";
        for (const string& state : states) cout << state << " ";
        cout << "\n Alphabet (E): ";
        for (char symbol : alphabet) cout << symbol << " ";
        cout << "\n Start State (q0): " << start_state;

        cout << "\n Accept States (F): ";
        for (const string& state : accept_states) cout << state << " ";
        cout << "\n Transitions (6):\n";
        for (const auto& [state, transitions_for_state] : transitions) {
            for (const auto& [symbol, next_state] : transitions_for_state) {
                cout << "  (" << state << ", " << symbol << ") -> " << next_state << "\n";
            }
        }
    }

    // Check if a word is accepted by the DFA
    bool CheckWord(const string& word) const {
        string current_state = start_state;
        for (char symbol : word) {
            if (alphabet.find(symbol) == alphabet.end() ||
                transitions.at(current_state).find(symbol) == transitions.at(current_state).end()) {
                return false; // Invalid symbol or missing transition
            }
            current_state = transitions.at(current_state).at(symbol);
        }
        return accept_states.find(current_state) != accept_states.end();
    }


    DeterministicFiniteAutomaton convertNFAtoDFA(const NFA& nfa) {
        set<string> dfaStates;                     // Q
        set<char> dfaAlphabet;                     // Σ
        map<string, map<char, string>> dfaTransitions; // δ
        string dfaStartState;                     // q0
        set<string> dfaAcceptStates;              // F

        // Lambda/Epsilon-Closure Computation
        auto computeEpsilonClosure = [&](const set<int>& states) {
            set<int> closure(states);
            queue<int> stateQueue;
            for (int state : states) stateQueue.push(state);

            while (!stateQueue.empty()) {
                int current = stateQueue.front();
                stateQueue.pop();

                if (nfa.transitions.count(current) && nfa.transitions.at(current).count('\0')) {
                    for (int next : nfa.transitions.at(current).at('\0')) {
                        if (closure.insert(next).second) {
                            stateQueue.push(next);
                        }
                    }
                }
            }
            return closure;
            };

        // Generate unique DFA state names
        int stateCounter = 0;
        auto getNewStateName = [&]() {
            return "q" + to_string(stateCounter++);
            };

        map<set<int>, string> nfaToDfaState;

        set<int> nfaStartSet = { nfa.start };
        set<int> startClosure = computeEpsilonClosure(nfaStartSet);
        string startStateName = getNewStateName();
        dfaStartState = startStateName;
        dfaStates.insert(startStateName);
        nfaToDfaState[startClosure] = startStateName;

        for (const auto& [state, transitions] : nfa.transitions) {
            for (const auto& [symbol, _] : transitions) {
                if (symbol != '\0') dfaAlphabet.insert(symbol);
            }
        }

        // Process DFA states
        queue<set<int>> stateQueue;
        stateQueue.push(startClosure);

        while (!stateQueue.empty()) {
            set<int> currentSet = stateQueue.front();
            stateQueue.pop();
            string currentDfaState = nfaToDfaState[currentSet];

            for (char symbol : dfaAlphabet) {
                set<int> reachableStates;

                for (int state : currentSet) {
                    if (nfa.transitions.count(state) && nfa.transitions.at(state).count(symbol)) {
                        for (int next : nfa.transitions.at(state).at(symbol)) {
                            reachableStates.insert(next);
                        }
                    }
                }

                set<int> closure = computeEpsilonClosure(reachableStates);

                if (!closure.empty()) {
                    string nextDfaStateName;
                    if (nfaToDfaState.count(closure)) {
                        nextDfaStateName = nfaToDfaState[closure];
                    }
                    else {
                        nextDfaStateName = getNewStateName();
                        nfaToDfaState[closure] = nextDfaStateName;
                        dfaStates.insert(nextDfaStateName);
                        stateQueue.push(closure);
                    }

                    dfaTransitions[currentDfaState][symbol] = nextDfaStateName;
                }
            }
        }

        for (const auto& [nfaStateSet, dfaStateName] : nfaToDfaState) {
            if (nfaStateSet.count(nfa.end)) {
                dfaAcceptStates.insert(dfaStateName);
            }
        }

        return DeterministicFiniteAutomaton(dfaStates, dfaAlphabet, dfaTransitions, dfaStartState, dfaAcceptStates);
    }


private:
    set<string> states;                     // Q
    set<char> alphabet;                     // Σ
    map<string, map<char, string>> transitions; // δ
    string start_state;                     // q0
    set<string> accept_states;              // F

};