#include "operator.hpp"

/**
 *  Implementation of the LTL operator WNext
 *
 */
class WeakNextOperator : Operator {

    public:
        int getOperator() {
            return SkeletonType::weaknext;
        }

        bool isBinary() {
            return false;
        }
    
        int getNumSkeletons(){
            return 1;
        }
        
        string printUnaryOperator(string lhs, bool normal) {
            return "( WeakX " + lhs + ")";
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            return "printed unaty as binary";
        }


        /**
         *  Creates Clauses for each timestep in the positive example for each timeStep.
         *
         *  in short such that "WNext" holds, the subformula has to hold in the next step.
         *  So we simply refer to a skeleton holding one higher timestep
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
        vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::weaknext);
            if (timeStep != maxLength -1) {
                for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                        int varETSAlphaPrimeId = dict.getVarEtsId(exampleId, timeStep+1, alphaSkeletonId);
                        vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaPrimeId};
                        clauses.push_back(clause);
                }
            }
            return clauses;
        }

        /**
         *  Creates Clauses for each timestep in the negative example for each timeStep.
         *
         *  in short such that "WNext" holds, the subformula may not hold in the next step.
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
        vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::weaknext);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                if (timeStep == maxLength -1) {
                    vector<int> clause  {-varETSId, -varSTId, -varAlphaId};
                    clauses.push_back(clause);
                } else {
                    int varETSAlphaPrimeId = dict.getVarEtsId(exampleId, timeStep + 1, alphaSkeletonId);
                    vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaPrimeId};
                    clauses.push_back(clause);
                }
            }
            return clauses;
        }
};
