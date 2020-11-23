#include "automaton.hpp"
#include "math.h"
#include <iostream>
#include <algorithm>
using namespace std;

int toAp(vector<int> v){
    int res = 0;
    for (int i = 0; i < v.size(); i++){
        if (v.at(i)!= 0) {
            res = res + pow(2,i);
        }
    }
    return res;
}

Automaton::Automaton(int numStates, int initialState, vector<string>facts){
    this->numStates = numStates;
    this->initialState = initialState;
    this->factsMap = facts;
    vector<vector<unordered_set<int> > > stateMap;
    for (int i = 0; i < numStates; i++) {
        vector<unordered_set<int> > stateToStates;
        for (int j = 0; j < pow(2,factsMap.size()); j++){
            unordered_set<int> targets;
            stateToStates.push_back(targets);
        }
        stateMap.push_back(stateToStates);
    }
    this->toNextState = stateMap;
}

int Automaton::fromPlanState(vector<string> planState) {
    int res = 0;
    for (int i = 0; i < factsMap.size(); i++){
        for (int j = 0; j < planState.size(); j++) {
            if (planState.at(j).compare(factsMap.at(i)) == 0){
                res += pow(2,i);
                break;
            }
        }
    }
    return res;
}

void Automaton::printAutomaton() {
    cout << "Initial State: " << this->initialState << endl;
    cout << "State Transitions: " << endl;
    for (int i = 0; i < this->toNextState.size(); i++) {
        for (int ap = 0; ap < this->toNextState.at(i).size(); ap++){
            cout << "State " << i << " with ap " << ap << " to [";
            for (int t : this->toNextState.at(i).at(ap)){
                cout << t << "," ;
            }
            cout << "]" << endl;
        }
    }
}
