#include "operator.hpp"

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

        vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            if (timeStep + 1 < maxLength){
                return genClauses(exampleId, timeStep, skeletonId, maxLength, fmlSize, dict);
            }
            return clauses;
        }
};
