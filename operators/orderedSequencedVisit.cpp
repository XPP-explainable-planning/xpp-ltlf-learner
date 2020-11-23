#include "operator.hpp"
#include <iostream>

class OrderedSequencedOperator: Operator {

    public:
        int getOperator() {
            return -1;
        }

        bool isBinary() {
            return true;
        }

        int getNumSkeletons(){
            return 2;
        }
    
        string printUnaryOperator(string lhs, bool normal) {
            return "printed binary operator as uinarty";
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            return "F (" + lhs + " & X F (" + rhs + ")) & ( ! " + lhs + " U " + rhs + " )";
        }

        void generateAdditionalVars(int skeletonId, VarsDict* dict){
            Variable* varST = new Variable(VarType::metaST);
            varST->skeletonId = skeletonId;
            varST->skeletonType = -1;
            //dict->addVariable(varST);
        }

        void generateAdditionalVarsForExample(int exampleId, int skeletonId, int maxLength,VarsDict* dict){
            for (int timeStep = 0; timeStep < maxLength; timeStep++){
                Variable* varETS = new Variable(VarType::metaETS);
                varETS->exampleId = exampleId;
                varETS->timestep = timeStep;
                varETS->skeletonId = skeletonId;
                varETS->alpha = 0;
                dict->addVariable(varETS);

                Variable* varETS1 = new Variable(VarType::metaETS);
                varETS1->exampleId = exampleId;
                varETS1->timestep = timeStep;
                varETS1->skeletonId = skeletonId;
                varETS1->alpha = 1;
                dict->addVariable(varETS1);

                Variable* varETS2 = new Variable(VarType::metaETS);
                varETS2->exampleId = exampleId;
                varETS2->timestep = timeStep;
                varETS2->skeletonId = skeletonId;
                varETS2->alpha = 2;
                dict->addVariable(varETS2);

                Variable* varETS3 = new Variable(VarType::metaETS);
                varETS3->exampleId = exampleId;
                varETS3->timestep = timeStep;
                varETS3->skeletonId = skeletonId;
                varETS3->alpha = 3;
                dict->addVariable(varETS3);

            }

        }

    
        virtual vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, -1);
                
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId ++){
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId ++){
            
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId); 
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);


                    //Enforce Alpha and Beta to be literals
                    int varSTLiteralAlpha = dict.getVarSTId(alphaSkeletonId, SkeletonType::literal);
                    int varSTLiteralBeta = dict.getVarSTId(betaSkeletonId, SkeletonType::literal);
                    vector<int> enforceBeta = {-varSTId, varSTLiteralBeta};
                    vector<int> enforceAlpha = {-varSTId, varSTLiteralAlpha};
                    clauses.push_back(enforceBeta);
                    clauses.push_back(enforceAlpha);

                    
                    int varUntil = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 3);

                    // !b U a part
                    if (timeStep + 1 < maxLength) {
                        vector<int> clauseAoB  {-varETSId, -varUntil, -varSTId, -varAlphaId, -varBetaId, varETSAlphaId, varETSBetaId};
                        clauses.push_back(clauseAoB);
                        int varUntilPrime = dict.getMetaEtsId(exampleId,timeStep + 1 ,skeletonId, 3);
                        vector<int> clauseUntilPrimeOB  {-varETSId, -varUntil, -varSTId, -varAlphaId, -varBetaId, varAlphaId, varUntilPrime};
                        clauses.push_back(clauseUntilPrimeOB);
                    } else if (timeStep + 1 == maxLength) {
                        vector<int> clauseAlpha  {-varETSId, -varUntil, -varSTId, -varAlphaId, varETSAlphaId};
                        clauses.push_back(clauseAlpha);
                    }




                    int varFirstFinally = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 0);
                    
                    //First Finally:
                    if (timeStep == maxLength - 1) {
                        vector<int> clause  {-varETSId, -varSTId,-varAlphaId, -varBetaId, varFirstFinally};
                        clauses.push_back(clause);
                    } else {
                        int varETSEventuallyPrime = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 0);
                        vector<int> clause  {-varETSId, -varSTId, -varAlphaId,-varBetaId,varFirstFinally, varETSEventuallyPrime};
                        clauses.push_back(clause);
                    }
                    
                    //a
                    vector<int> clauseLhs {-varFirstFinally,-varSTId, -varAlphaId, varETSAlphaId};
                    clauses.push_back(clauseLhs);

                    //X F b
                    int varNextId = dict.getMetaEtsId(exampleId, timeStep, skeletonId, 1);
                    if (timeStep == maxLength - 1) {
                        vector<int> clause  {-varFirstFinally, -varSTId, -varBetaId};
                        clauses.push_back(clause);
                    } else {
                        int varETSNextId = dict.getMetaEtsId(exampleId, timeStep+1, skeletonId, 1);
                        vector<int> clause  {-varFirstFinally, -varSTId,-varBetaId, varETSNextId};
                        clauses.push_back(clause);
                    }
                    
                    //F b
                    if (timeStep == maxLength - 1) {
                        //int varFinally = dict.getMetaEtsId(exampleId, timeStep, skeletonId, 2);
                        vector<int> clause  {-varNextId, -varSTId, -varBetaId, varETSBetaId};
                        clauses.push_back(clause);
                    } else {
                        int varFinally = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 1);
                        vector<int> clause  {-varNextId, -varSTId, -varBetaId, varETSBetaId ,varFinally };
                        clauses.push_back(clause);
                    }
                }
            }
                    
            return clauses;
        }

        virtual vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, -1);
                
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId ++){
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId ++){
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId); 
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);

                    int varUntil = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 3);

                    if (timeStep + 1 < maxLength) {
                        //either ((aRb)' and b) or (a and b) \equiv b and ( a or (aRb)' )
                        vector<int> clauseBeta  {-varUntil, -varSTId, -varAlphaId, varETSAlphaId};
                        clauses.push_back(clauseBeta);
                        int varUntilPrime = dict.getMetaEtsId(exampleId,timeStep + 1,skeletonId, 3);
                        vector<int> clauseReleasePrimeOrAlpha = {-varUntil, -varSTId, -varBetaId, varETSBetaId, varUntilPrime};
                        clauses.push_back(clauseReleasePrimeOrAlpha);

                    } else if (timeStep + 1 == maxLength) {
                        vector<int> clauseBeta  {-varUntil, -varSTId, -varAlphaId, varETSAlphaId};
                        clauses.push_back(clauseBeta);
                    }

                    //Fb & U
                    int varFirstFinally = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 0);
                    vector<int> clauseFin {-varETSId,-varSTId,-varAlphaId,-varBetaId, varFirstFinally, varUntil};
                    clauses.push_back(clauseFin);
                        
                    if (timeStep + 1 < maxLength) {
                        int varETSEventuallyPrime = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 0);
                        vector<int> clause  {-varETSId, -varSTId,-varAlphaId, -varBetaId, -varFirstFinally, varETSEventuallyPrime};
                        clauses.push_back(clause);
                    }
                    
                    //X F b
                    int varNextId = dict.getMetaEtsId(exampleId, timeStep, skeletonId, 1);
                    //a & XF B
                    vector<int> andClause {-varFirstFinally,-varSTId,-varAlphaId,-varBetaId, varETSAlphaId, varNextId};
                    clauses.push_back(andClause);

                    //XFB
                    if (timeStep + 1 < maxLength) { 
                        int varETSNextId = dict.getMetaEtsId(exampleId, timeStep+1, skeletonId, 1);
                        vector<int> clause  {-varFirstFinally, -varSTId,-varBetaId, varETSNextId};
                        clauses.push_back(clause);
                    }
                    
                    //F b
                    vector<int> clause  {-varNextId, -varSTId, -varBetaId, varETSBetaId};
                    clauses.push_back(clause);
                    if (timeStep + 1 < maxLength) {
                        int varFinally = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 1);
                        vector<int> clause  {-varNextId, -varSTId, -varBetaId, varFinally };
                        clauses.push_back(clause);
                    }
                }
            }
    
            return clauses;
        }
};


