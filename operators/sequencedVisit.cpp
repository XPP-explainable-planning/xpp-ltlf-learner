#include "operator.hpp"
#include <iostream>

class SequencedVisitOperator : Operator {

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
            return "F (" + lhs + " & X F (" + rhs + "))";
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

            int varFirstFinally = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 0);
            int varAndLhs = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 1);
            int varAndRhsX = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 2);
            int varSecondFinally = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 3);


            //First Finally:
            if (timeStep == maxLength - 1) {
                vector<int> clause  {-varETSId, -varSTId, varFirstFinally};
                clauses.push_back(clause);
            } else {
                int varETSEventuallyPrime = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 0);
                vector<int> clause  {-varETSId, -varSTId, varFirstFinally, varETSEventuallyPrime};
                clauses.push_back(clause);
            }
            
            //&
            //
            vector<int> lhsAndClause {-varFirstFinally, varAndLhs};
            clauses.push_back(lhsAndClause);
            vector<int> rhsAndClause {-varFirstFinally, varAndRhsX};
            clauses.push_back(rhsAndClause);
            
            vector<int> lhsLiteral {-varAndLhs, -varAlphaId, varETSAlphaId};
            clauses.push_back(lhsLiteral);
            //X F b
            if (timeStep == maxLength - 1) {
                vector<int> clause  {-varAndRhsX, -varSTId, -varSecondFinally};
                clauses.push_back(clause);
            } else {
                int varETSFinally = dict.getMetaEtsId(exampleId, timeStep+1, skeletonId, 3);
                vector<int> clause  {-varAndRhsX, -varSTId, varETSFinally};
                clauses.push_back(clause);
            }
            
            //F b
            if (timeStep == maxLength - 1) {
                //int varFinally = dict.getMetaEtsId(exampleId, timeStep, skeletonId, 2);
                vector<int> clause  {-varSecondFinally, -varBetaId, varETSBetaId};
                clauses.push_back(clause);
            } else {
                int varFinally = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 3);
                vector<int> clause  {-varSecondFinally, -varSTId, -varBetaId, varETSBetaId ,varFinally };
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
            int varFirstFinally = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 0);
          
            int varAndLhs = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 1);
            int varAndRhsX = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 2);
            int varSecondFinally = dict.getMetaEtsId(exampleId,timeStep,skeletonId, 3);

            //F(a & XFb)
            vector<int> clauseFin {-varETSId,-varSTId, varFirstFinally};
            clauses.push_back(clauseFin);
                
            if (timeStep + 1 < maxLength) {
                int varETSEventuallyPrime = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 0);
                vector<int> clause  {-varETSId, -varSTId, varETSEventuallyPrime};
                clauses.push_back(clause);
            }

            //a & XFB
            vector<int> andClause {-varFirstFinally,varAndLhs, varAndRhsX};
            clauses.push_back(andClause);
            
            vector<int> lhsLiteral = {-varAndLhs, -varAlphaId, varETSAlphaId};
            clauses.push_back(lhsLiteral);
            //XFB
            if (timeStep + 1 < maxLength) { 
                int varETSFinally = dict.getMetaEtsId(exampleId, timeStep+1, skeletonId, 3);
                vector<int> clause  {-varAndRhsX, -varSTId, varETSFinally};
                clauses.push_back(clause);
            }
            
            //F b
            vector<int> clause  {-varSecondFinally, -varBetaId, varETSBetaId};
            clauses.push_back(clause);
            if (timeStep + 1 < maxLength) {
                int varFinally = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, 3);
                vector<int> clause  {-varSecondFinally, -varBetaId, varFinally };
                clauses.push_back(clause);
            }
            }}
                    
            return clauses;
        }
};


