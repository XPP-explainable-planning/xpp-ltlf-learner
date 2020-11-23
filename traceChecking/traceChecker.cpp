#include <iostream>
#include "../definitions.hpp"
#include "automaton.hpp"
#include <fstream>
#include <sstream>
#include "math.h"
#include <unordered_set>

using namespace std;


void addTransitions(Automaton* at,int current, int target, int numFacts,
                          vector<int> mustHold, vector<int> mustNotHold) {
    
    
    if (mustHold.size() + mustNotHold.size() == numFacts){
        int res = 0;
        for (int a : mustHold){
            res = res + pow(2,a);
        }
        at->toNextState.at(current).at(res).insert(target);
    }
    else {
        for (int i = 0; i < numFacts; i++){
            //vectors should be sorteted
            bool found = false;
            for (int holds : mustHold){
                if (i == holds){
                    found = true;
                    break;
                }
            }
            if (!found) {
                for (int holds : mustNotHold){
                    if (i == holds){
                        found = true;
                        break;
                    }
                }
                if (!found){
                    vector<int> f = mustHold;
                    f.push_back(i);
                    addTransitions(at, current, target, numFacts, f, mustNotHold);
                    f = mustNotHold;
                    f.push_back(i);
                    addTransitions(at, current, target, numFacts, mustHold, f);
                    return;
                }
            }   
                
        }
    }
}


bool checkForTrace(Automaton* at, vector<vector<string> > plan) {
    unordered_set<int> currentLvl;   
    currentLvl.insert(at->initialState);
    for (int pos = 0; pos < plan.size(); pos++){
        unordered_set<int> nextLvl;   
        int planStateAp = at->fromPlanState(plan.at(pos));
        for (int currentState : currentLvl){
            for (int targetState : at->toNextState.at(currentState).at(planStateAp)){
                nextLvl.insert(targetState);
            }
        }
        currentLvl = nextLvl;
    }
    for (int resultingState : currentLvl){
        if (at->finalStates.find(resultingState)!= at->finalStates.end())
            return true;
    }
    return false;
}


Automaton *automatonFromHoaFile(const string &filename) {
    string line;
    Automaton *automaton = nullptr;
//    printf("now parsing file %s\n",filename.c_str());
    ifstream myfile(filename);
    if (myfile.is_open()) {
        getline(myfile, line);
        getline(myfile, line);
        getline(myfile, line);
        //Read num States
        int numStates;
        sscanf(line.c_str(), "States: %i", &numStates);
        //get StartState
        unsigned long start;
        getline(myfile, line);
        sscanf(line.c_str(), "Start: %lu", &start);

        //get aps to map them to the ours. AP: n ["."]*n
        getline(myfile, line);
        //cout << line << endl;
        string buf;
        stringstream stream(line);
        vector<string> tokens;
        while (stream >> buf){
            if (buf[0] == '\"') {
                buf.erase(0,1);
                buf.pop_back();
                tokens.push_back(buf);
            }
        }
        
         automaton = new Automaton(numStates,start, tokens);
        
            //   printf("position of ap %s is %d\n",buf.c_str(),position);
        //accname
        //first state:
        getline(myfile, line);
        while (line != "--BODY--") {
            getline(myfile, line);
        }

        getline(myfile, line);
        int currentState = -1;
        while (line != "--END--") {
            //   printf("current line is : %s\n",line.c_str());
            if (line[0] == 'S') {
                currentState++ ;
                stringstream statestream(line);
                statestream >> buf;
                while (statestream >> buf) {
                    if (buf[0] == '{') {
                        automaton->finalStates.insert(currentState);
                    } 
                }
            } else {
                stringstream edgestream(line);
                getline(edgestream, buf, '[');
                getline(edgestream, buf, '[');
                stringstream aps(buf);
                getline(aps, buf, ']');
                string targetState;
                getline(aps, targetState, ']');
                stringstream apstream(buf);
                int target = atoi(targetState.c_str());
                vector<int> mustBeFullfilled;
                vector<int> mayNotContain;
                while (apstream >> buf) {
                    if (buf[0] == 't') {
                        addTransitions(automaton, currentState, target, tokens.size(), mustBeFullfilled,mayNotContain);
                    } else if (buf[0] == 'e') {
      //                  cout << "no" << endl;
                    } else if (buf[0] == '|') {
                        mustBeFullfilled.clear();
                        mayNotContain.clear();
                    } else {
                        stringstream apStream(buf);
                        string ap;
                        while (getline(apStream, ap, '&')) {
                            if (ap[0] == '!') {
                                ap = ap.substr(1, ap.size() - 1);
    //                            cout << "AP is " << ap << endl;
                                mayNotContain.push_back(atoi(ap.c_str()));
                            } else {
                                mustBeFullfilled.push_back(atoi(ap.c_str()));
                            }
                        }
                        //                       printf("adding transitions for line: %s\n",line.c_str());
                        addTransitions(automaton,currentState, target,tokens.size(), mustBeFullfilled, mayNotContain);
                    }
                } 
            }
            getline(myfile, line);
        }
    }
    myfile.close();
    return automaton;
}


Automaton* generateAutomaton(string formula)
{
  string call = "ltl2tgba -C -B -D \'" + formula + "\' > a.txt";
  //cout << call <<endl;
  system(call.c_str());
  return automatonFromHoaFile("a.txt");
}


