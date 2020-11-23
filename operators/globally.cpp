#include "operator.hpp"

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
