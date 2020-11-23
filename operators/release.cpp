#include "operator.hpp"

class ReleaseOperator : Operator {

    public:
        int getOperator() {
            return SkeletonType::release;
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
                return "( " + lhs + " R " + rhs + " )";
            return "R " + lhs + " " + rhs;
        }

        vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::release);
            for (int alphaSkeletonId = skeletonId + 1 ; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    if (timeStep + 1 < maxLength) {
                        vector<int> clauseBeta  {- varETSId, -varSTId, -varBetaId, varETSBetaId};
                        clauses.push_back(clauseBeta);

                        int varETsReleasePrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                        vector<int> clauseReleaseOrAlpha   {- varETSId, -varSTId, -varAlphaId, varETSAlphaId, varETsReleasePrime};
                        clauses.push_back(clauseReleaseOrAlpha);
                    } 
                    else if (timeStep == maxLength -1) {
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
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::release);
            for (int alphaSkeletonId = skeletonId + 1 ; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) {
                    int varAlphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
                    int varBetaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
                    
                    int varETSAlphaId = dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId);
                    int varETSBetaId = dict.getVarEtsId(exampleId, timeStep, betaSkeletonId);
                    if (timeStep + 1 < maxLength) {

                        // either ((aUb)' and a) or b \equiv ((aUb)' or b) and (a or b)
                        vector<int> clauseBeta  {- varETSId, -varSTId, -varAlphaId, -varBetaId, varETSAlphaId, varETSBetaId};
                        clauses.push_back(clauseBeta);
                        int varETSUntilPrime = dict.getVarEtsId(exampleId, timeStep + 1, skeletonId);
                        vector<int> clauseReleaseOrAlpha   {- varETSId, -varSTId, -varAlphaId, -varBetaId, varETSBetaId , varETSUntilPrime};
                        clauses.push_back(clauseReleaseOrAlpha);
                    } 
                    else if (timeStep == maxLength -1) {
                        vector<int> clauseBeta  {-varETSId, -varSTId, -varBetaId, varETSBetaId};
                        clauses.push_back(clauseBeta);

                    }
                }
            }
            return clauses;
        }
};
