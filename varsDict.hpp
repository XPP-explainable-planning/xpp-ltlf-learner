#ifndef VARS_DICT_HPP
#define VARS_DICT_HPP

#include "variable.hpp"
#include "definitions.hpp"
#include <vector>
#include <unordered_map>
using namespace std;

class VarsDict{
    private: 
        unordered_map<int,unordered_map<int,unordered_map<int,Variable*> > > examples;
        unordered_map<int,unordered_map<int,Variable*> > skeletons;
        unordered_map<int,unordered_map<int,Variable*> > alphas;
        unordered_map<int,unordered_map<int,Variable*> > betas;
        unordered_map<int,unordered_map<int,Variable*> > exliterals;
        unordered_map<int,unordered_map<int,Variable*> > skliterals;
        unordered_map<int,unordered_map<int,unordered_map<int,unordered_map<int,Variable*> > > > metaETS;
        unordered_map<int,unordered_map<int,Variable*> > metaSkeletons;
       // unordered_map<int,unordered_map<int,unordered_map<int,Variable>>> expredicates;

    public :
        vector<string> vocabulary;
        vector<Variable*> lookupTable;
        int addVariable(Variable* variable);
        int getVarEtsId(int exampleId, int timestemp, int skeletonId);
        int getVarSTId(int skeletonId, int skeletonType);
        int getVarEtlId(int exampleId, int timestamp, int literal);
        int getVarAlphaId(int skeletonId, int alphaSkeletonId);
        int getVarBetaId(int skeletonId, int betaSkeletonId);
        int getVarLitId(int skeleltonId, int literal);
        int getMetaEtsId(int exampleId, int timestamp,int skeletonId, int pos);
        int getMetaStId(int skeletonId, int skeletonType);
       // int getVarExId(int exampleId, int timestamp, int predicate);
};


#endif
