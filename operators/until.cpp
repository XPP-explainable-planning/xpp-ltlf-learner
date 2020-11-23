#include "operator.hpp"

class UntilOperator : Operator {

    public:
        int getOperator() {
            return SkeletonType::until;
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
                return "( " + lhs + " U " + rhs + " )";
            return "U " + lhs + " " + rhs;
        }

        vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::until);
            for (int alphaSkeletonId = skeletonId + 1 ; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);

                    if (timeStep + 1 < maxLength) {
                        vector<int> clauseAoB  {-varETSId, -varSTId, -varAlphaId, -varBetaId, varETSAlphaId, varETSBetaId};
                        clauses.push_back(clauseAoB);
                        int varETSUntilPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                        vector<int> clauseUntilPrimeOB  {-varETSId, -varSTId, -varAlphaId, -varBetaId, varETSBetaId, varETSUntilPrime};
                        clauses.push_back(clauseUntilPrimeOB);
                    } else if (timeStep + 1 == maxLength) {
                        vector<int> clauseBeta  {-varETSId, -varSTId, -varBetaId, varETSBetaId};
                        clauses.push_back(clauseBeta);
                    }
                }
            }
            return clauses;
        }

        vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::until);
            for (int alphaSkeletonId = skeletonId + 1 ; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) {

                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);

                    if (timeStep + 1 < maxLength) {
                        //either ((aRb)' and b) or (a and b) \equiv b and ( a or (aRb)' )
                        vector<int> clauseBeta  {-varETSId, -varSTId, -varBetaId, varETSBetaId};
                        clauses.push_back(clauseBeta);
                        int varETSReleasePrimeId = dict.getVarEtlId(exampleId, timeStep+1, skeletonId);
                        vector<int> clauseReleasePrimeOrAlpha = {-varETSId, -varSTId, -varAlphaId, varETSAlphaId, varETSReleasePrimeId};
                        clauses.push_back(clauseReleasePrimeOrAlpha);

                    } else if (timeStep + 1 == maxLength) {
                        vector<int> clauseBeta  {-varETSId, -varSTId, -varBetaId, varETSBetaId};
                        clauses.push_back(clauseBeta);
                    }
                }
            }
            return clauses;
        }
};
