#include "operator.hpp"

class GloballyAndOperator: Operator {
    int metaId;
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
            return "Printed binary g&& as unary";
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            return "(G ( " + lhs + " & " + rhs + " ))";
        }

        vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, -1);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId};
                    vector<int> clause2  {-varETSId, -varSTId, -varBetaId, varETSBetaId};
                    clauses.push_back(clause);
                    clauses.push_back(clause2);
                    if (timeStep + 1 < maxLength){ 
                        int varETSAlwaysPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                        vector<int> cornerCase  {-varETSId, -varSTId, -varAlphaId, varETSAlwaysPrime};        
                        vector<int> cornerCase2  {-varETSId, -varSTId, -varBetaId, varETSAlwaysPrime};        
                        clauses.push_back(cornerCase);
                    }
                }
            }

            return clauses;
        }

        vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, -1);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) { 
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    if (timeStep == maxLength - 1) {
                        vector<int> cl = {-varETSId, -varSTId, -varAlphaId, -varBetaId, varETSBetaId, varETSAlphaId};
                        clauses.push_back(cl);
                    } else {
                        int varETSAlwaysPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                        vector<int> cornerCase  {-varETSId, -varSTId, -varAlphaId,-varBetaId, varETSBetaId, varETSAlphaId, varETSAlwaysPrime};
                        clauses.push_back(cornerCase);
                    }
                }
            }
            return clauses;
        }
};
