#ifndef CNF_HPP
#define CNF_HPP

#include<vector>
#include "varsDict.hpp"
#include "operators/operator.hpp"
#include <minisat/core/Solver.h>
using namespace std;
using namespace Minisat;
class Cnf {
    
    private:
        bool printNormal;
        int numClausesAfterInit;
        int traceLength;
        int numVariablesAfterInit;
        int numClausesBeforeLearning;
        int additionalVarPerExample = 0;
        vector<int> numClausesPerTrace;
        vector<int> numVariablesPerTrace;
        vector<int> learnClause;
        void initOperators(vector<Operator*> metaOperators);
        void atMostOne(vector<int> ids);
        void exactlyOne(vector<int> ids);
        string learnFormula();
        vector<Operator*> operators;
        vector<int> AllSkeletonTypes;
        Operator* getOperatorForType(int type);
        void genClausesLiteral(int exampleId, int timeStep, int skeletonId, vector<string> observation);
        
        void genDualClausesLiteral(int exampleId, int timeStep, int skeletonId, vector<string> observation);
    public: 
        void setOperators(vector<SkeletonType> ops);
        Cnf (int fmlSize, vector<string> vocabulary, vector<Operator*> metaOperator, bool normal);
        VarsDict dict;
        int fmlSize;
        int numExamples;
        vector<vector<int> > clauses;
        string recoverFormulafromModel(vector<int> model);
        void addPositiveExample(vector<vector<string> >);
        void addNegativeExample(vector<vector<string> >);
        void genInitialVariables();
        vector<string> learnFormulas(int formulaCount);

        void genInitialClauses();
        void genVariablesForExample(vector<vector<string> > example);        
        void genSkeletonClausesForPosExample(vector<vector<string> > example);        
        void genSkeletonClausesForNegExample(vector<vector<string> > example);        
        int getSkeletonType(vector<int> model, int skeletonId);
        int getAlphaId(vector<int> model, int skeletonId);
        int getBetaId(vector<int> model, int skeletonId);
        string getLiteral(vector<int> model, int skeletonId);
        string skeletonToFormula(vector<int> model, int skeletonId);
        string negatePredicate(string s);
        void printStatistics();
        void printTable();
};


#endif
