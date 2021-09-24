#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include "../definitions.hpp"
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;


/**
 *
 *  Basic automaton structure for checking if a plan is satisfied by a formula
 *
 */ 
class Automaton {
    
    public:
        Automaton(int numStates, int initialState, vector<string> facts);
    int numStates;
    int initialState;
    vector<string> factsMap;
    vector<vector<unordered_set<int>>> toNextState;
    int fromPlanState(vector<string> planState);
    unordered_set<int> finalStates;
    void printAutomaton();
};  


bool checkForTrace(Automaton* automaton, vector<vector<string> > plan);
Automaton* generateAutomaton(string formula);
#endif
