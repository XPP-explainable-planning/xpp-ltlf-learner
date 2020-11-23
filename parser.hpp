#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

class Parser {
    private: 
        bool stateBased;
        vector<string> ignorePredicates;
        unordered_set<string> vocabulary;
    public: 
    bool checkIgnore(string current);
    vector<string> getVocabulary();  
    Parser(bool stateBased, vector<string> ignorePredicates):stateBased(stateBased),ignorePredicates(ignorePredicates){}
    vector<vector<string>> parse(char* filename);
    vector<vector<string>> parseStateTrace(char* filename);
    vector<vector<string>> parseTrace(char* filename);
};

#endif 
