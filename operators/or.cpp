#include "operator.hpp"

class OrOperator : public Operator {

    public:
        int getOperator() {
            return SkeletonType::lor;
        }

        bool isBinary() {
            return true;
        }
        
        int getNumSkeletons(){
            return 2;
        }

        string printUnaryOperator(string lhs, bool normal) {
            return "Printed binary or as unary";
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            if (normal)
                return "( " + lhs + " | " + rhs + " )";
            return "|| " + lhs + " " + rhs;
        }

        vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::lor);
            for (int alphaSkeletonId = skeletonId + 1 ; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    vector<int> clause  {-varETSId, - varSTId, - varAlphaId, - varBetaId, varETSAlphaId, varETSBetaId};
                    clauses.push_back(clause);
                }
            }
            return clauses;
        }

        vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::lor);
            for (int alphaSkeletonId = skeletonId + 1 ; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    vector<int> clause  {-varETSId, -varSTId, -varAlphaId, varETSAlphaId};
                    vector<int> clause2  {-varETSId, -varSTId, -varBetaId, varETSBetaId};
                    clauses.push_back(clause);
                    clauses.push_back(clause2);
                }
            }
            return clauses;
        }
};
