#include "operator.hpp"

/**
 *
 *  Implementation of the LTL Operator and
 *
 *
 */ 
class AndOperator : public Operator {
    
        int getOperator() {
            return SkeletonType::land;
        }

        bool isBinary() {
            return true;
        }
    
        int getNumSkeletons(){
            return 2;
        }

        string printUnaryOperator(string lhs, bool normal) {
            return "printed binary and as unary";
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            if (normal)
                return "( " + lhs + " & " + rhs + " )";
            return "&& " + lhs + " " + rhs; 
        }
        
        /**
         *  Creates Clauses for each timestep in the positive example for each timeStep.
         *
         *  in short such that "AND" holds, both subformulas have to hold, so alpha and beta runs have to be satisfied, we add
         *  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId}, {-varETSId, -varSTId, -varBetaId, varETSBetaId}
         *  
         *  @param exampleId - the id of the current positive plan
         *  @param timestep - the current timestep in the plan
         *  @param skeletonId - the skeletonId for the node where we implement the operator
         *  @param maxLength - the maximal number of steps in the plan
         *  @param fmlSize - the maximum formula size 
         *  @param dict - the dictionary to get information about the variables.
         *
         *
         */
        virtual vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::land);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) { 
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    vector<int> clause1  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId};
                    vector<int> clause2  {-varETSId, -varSTId, -varBetaId, varETSBetaId};
                    clauses.push_back(clause1);
                    clauses.push_back(clause2);
                }
            }
            return clauses;
        }

        /**
         *  Creates Clauses for each timestep in the negative example for each timeStep.
         *
         *  in short such that "AND" does not hold, one of the subformula has not to hold, so either alpha or beta runs has to be dissatisfied, we add
         *  {-varETSId, -varSTId, -varAlphaId,-varBetaId, varETSAlphaId, varETSBetaId}
         *  to the clauses  
         *  @param exampleId - the id of the current positive plan
         *  @param timestep - the current timestep in the plan
         *  @param skeletonId - the skeletonId for the node where we implement the operator
         *  @param maxLength - the maximal number of steps in the plan
         *  @param fmlSize - the maximum formula size 
         *  @param dict - the dictionary to get information about the variables.
         *
         *
         */
        virtual vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::land);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) { 
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    vector<int> clause1  {-varETSId, -varSTId, -varAlphaId,-varBetaId, varETSAlphaId, varETSBetaId};
                    clauses.push_back(clause1);
                }
            }
            return clauses;
        }
};


