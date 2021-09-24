#ifndef OPERATOR_TEMPLATE_HPP
#define OPERATOR_TEMPLATE_HPP
#include "../../definitions.hpp"
#include "../../varsDict.hpp"
#include <string>

using namespace std;
    
/**
 *  Variable package, used as struct to forward variable ids
 *
 */ 
    struct VarPackage {
        int ets;
        int st;
        int alpha;
        int beta;
        int etsPrime;
        int alphaPrime;
    };


/**
 *  
 *  TreeStructure like class for implementing the inner tree of metaoperators
 */ 
class OperatorTemplate {
    public: 
        OperatorTemplate(SkeletonType type, bool isUnary, int leaf):type(type),isUnary(isUnary),leaf(leaf){};
        SkeletonType type;
        bool isUnary;
        int leaf;
        int varId;
        OperatorTemplate* lhs;
        OperatorTemplate* rhs;
        string printMe(string lhs, string rhs, bool normal);
        void genVariablesForExamples(int exampleId, int skeletonId, int maxLength, VarsDict* dict, int myId);
        int getMetaId();
};

vector<vector<int> > genClausesForTemplates(SkeletonType type,int timeStep, int maxLength, VarPackage vars);
vector<vector<int> > genDualClausesForTemplates(SkeletonType type,int timeStep, int maxLength, VarPackage vars);
    

#endif
