#include "operator.hpp"

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
