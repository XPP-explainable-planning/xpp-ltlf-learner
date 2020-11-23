#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "../definitions.hpp"
#include "../varsDict.hpp"
#include <string>

using namespace std;



class Operator {
    public: 
        int addVarsPExample = 0;
        int isMeta = false;
        virtual int getOperator() = 0; 
        virtual bool isBinary() = 0;
        virtual int getNumSkeletons() = 0;
        virtual string printUnaryOperator(string rhs, bool normal) = 0;
        virtual string printBinaryperator(string lhs, string rhs, bool normal) = 0;
        virtual void generateAdditionalVars(int skeletonId,VarsDict* dict){}
        virtual void generateAdditionalVarsForExample(int exampleId, int skeletonId, int maxLength,VarsDict* dict){}
        virtual vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) = 0;
        virtual vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) = 0;

};

Operator* createOperator(int operatorType);

#endif
