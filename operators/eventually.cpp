#include "operator.hpp"
#include <iostream>
class EventuallyOperator : Operator {

    public:
        int getOperator() {
            return SkeletonType::eventually;
        }

        bool isBinary() {
            return false;
        }
    
        int getNumSkeletons(){
            return 1;
        }

        string printUnaryOperator(string lhs, bool normal) {
            if (normal)
                return "( F " + lhs + " )";
            return "F " + lhs;
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            return "printed unaty as binary";
        }

        vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::eventually);
                for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    if (timeStep == maxLength - 1) {
                        vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId};
                        clauses.push_back(clause);
                    } else {
                        int varETSEventuallyPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                        vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId, varETSEventuallyPrime};
                        clauses.push_back(clause);
                    }
                }
            return clauses;
        }

        vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::eventually);
                for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId};
                    clauses.push_back(clause);
                    if (timeStep + 1 < maxLength ){
                        int varETSEventuallyPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                        vector<int> clause2 {-varETSId, -varSTId, -varAlphaId, varETSEventuallyPrime};
                        clauses.push_back(clause2);
                    }
                }
            return clauses;
        }
};
