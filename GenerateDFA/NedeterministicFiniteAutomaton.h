#pragma once
#include <iostream>
#include "PolishForm.h"
#include "map"
#include "vector"

struct NFA {
    int start;
    int end;
    map<int, map<char, vector<int>>> transitions;
};


class NondeterministicFiniteAutomaton {
public:
    NFA createNFAFromPostfix(const string& postfix) {
        stack<NFA> nfaStack;
        int stateCounter = 0;

        for (char character : postfix) {
            if (isalnum(character)) { // Operand
                NFA nfa;
                nfa.start = stateCounter++;
                nfa.end = stateCounter++;
                nfa.transitions[nfa.start][character].push_back(nfa.end);
                nfaStack.push(nfa);
            }
            else if (character == '*') { // Kleene Star
                NFA nfa = nfaStack.top(); nfaStack.pop();
                NFA newNFA;
                newNFA.start = stateCounter++;
                newNFA.end = stateCounter++;
                newNFA.transitions[newNFA.start]['\0'].push_back(nfa.start);
                newNFA.transitions[newNFA.start]['\0'].push_back(newNFA.end);
                newNFA.transitions[nfa.end]['\0'].push_back(nfa.start);
                newNFA.transitions[nfa.end]['\0'].push_back(newNFA.end);
                newNFA.transitions.insert(nfa.transitions.begin(), nfa.transitions.end());
                nfaStack.push(newNFA);
            }
            else if (character == '|') { // Alternation
                NFA nfa2 = nfaStack.top(); nfaStack.pop();
                NFA nfa1 = nfaStack.top(); nfaStack.pop();
                NFA newNFA;
                newNFA.start = stateCounter++;
                newNFA.end = stateCounter++;
                newNFA.transitions[newNFA.start]['\0'].push_back(nfa1.start);
                newNFA.transitions[newNFA.start]['\0'].push_back(nfa2.start);
                newNFA.transitions[nfa1.end]['\0'].push_back(newNFA.end);
                newNFA.transitions[nfa2.end]['\0'].push_back(newNFA.end);
                newNFA.transitions.insert(nfa1.transitions.begin(), nfa1.transitions.end());
                newNFA.transitions.insert(nfa2.transitions.begin(), nfa2.transitions.end());
                nfaStack.push(newNFA);
            }
            else if (character == '.') { // Concatenation
                NFA nfa2 = nfaStack.top(); nfaStack.pop();
                NFA nfa1 = nfaStack.top(); nfaStack.pop();
                nfa1.transitions[nfa1.end]['\0'].push_back(nfa2.start);
                nfa1.end = nfa2.end;
                nfa1.transitions.insert(nfa2.transitions.begin(), nfa2.transitions.end());
                nfaStack.push(nfa1);
            }
        }

        return nfaStack.top();
    }

    void outputTransitions(const NFA& nfa) {
        for (const auto& startState : nfa.transitions) {
            for (const auto& transition : startState.second) {
                char symbol = transition.first;
                for (int endState : transition.second) {
                    cout << "Transition: (" << startState.first << " --" << symbol << "--> " << endState << ")\n";
                }
            }
        }
    }


};
