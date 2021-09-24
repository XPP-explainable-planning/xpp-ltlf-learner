#include "operator.hpp"


/**
 *  
 *  Implementation of the LTL Operator Globally
 *
 */ 
class AlwaysOperator : Operator {

    public:
        int getOperator() {
            return SkeletonType::always;
        }

        bool isBinary() {
            return false;
        }

        int getNumSkeletons(){
            return 1;
        }
    
        string printUnaryOperator(string lhs, bool normal) {
            if (normal)
                return "( G " + lhs + " )";
            return "G " + lhs;
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            return "printed unaty as binary";
        }


        /**
         *  Creates Clauses for each timestep in the positive example for each timeStep.
         *
         *  in short such that "Globally" holds, we have a & XG a
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
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::always);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId};
                clauses.push_back(clause);
                if (timeStep + 1 < maxLength){ 
                    int varETSAlwaysPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                    vector<int> cornerCase  {-varETSId, -varSTId, -varAlphaId, varETSAlwaysPrime};        
                    clauses.push_back(cornerCase);
                }
            }
            return clauses;
        }


        /**
         *  Creates Clauses for each timestep in the negative example for each timeStep.
         *
         *  in short such that "Globally" does not hold, we have that at either !a | XF!a
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
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::always);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                if (timeStep == maxLength - 1) {
                    vector<int> cl = {-varETSId, -varSTId,-varAlphaId, varETSAlphaId};
                    clauses.push_back(cl);
                } else {
                    int varETSAlwaysPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                    vector<int> cornerCase  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId, varETSAlwaysPrime};
                    clauses.push_back(cornerCase);
                }
            }
            return clauses;
        }
};
