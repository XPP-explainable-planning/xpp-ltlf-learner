#include "operator.hpp"

/**
 *  Operator implementation of the Operator Next
 *
 */
class NextOperator : Operator {

    public:
        int getOperator() {
            return SkeletonType::next;
        }

        bool isBinary() {
            return false;
        }

        int getNumSkeletons(){
            return 1;
        }

    
        string printUnaryOperator(string lhs, bool normal) {
            if (normal)
                return "( X " + lhs + " )";
            return "X " + lhs; 
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            return "printed unaty as binary";
        }


        /**
         *  Creates Clauses for each timestep in the positive example for each timeStep.
         *
         *  in short such that "Next" holds, the subformula has to hold in the next step.
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
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::next);

            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                
                if (timeStep == maxLength - 1) {
                    vector<int> clause  {-varETSId, -varSTId, -varAlphaId};

                    clauses.push_back(clause);
                } else {
                    int varETSAlphaPrimeId = dict.getVarEtsId(exampleId, timeStep+1, alphaSkeletonId);
                    vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaPrimeId};
                    clauses.push_back(clause);
                }
            }
            return clauses;
        }

        /**
         *  Creates dualClauses for each timestep in the negative example for each timeStep.
         *
         *  in short such that "Next" holds, the subformula has to hold in the next step.
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
        vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            if (timeStep + 1 < maxLength){
                return genClauses(exampleId, timeStep, skeletonId, maxLength, fmlSize, dict);
            }
            return clauses;
        }
};
