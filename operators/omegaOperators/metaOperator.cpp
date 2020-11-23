#include "../operator.hpp"
#include "operatorTemplate.hpp"

class MetaOperator : public Operator {

        int metaType;
        bool hasBeta;
        OperatorTemplate* operatorTemplate;

        int getOperator() {
            return metaType; 
        }

        bool isBinary() {
            return hasBeta;
        }
    
        int getNumSkeletons(){
                return 2;
        }

        string printUnaryOperator(string lhs, bool normal) {
            return operatorTemplate->printMe(lhs, "ERROR", normal);
        }

        string printBinaryperator(string lhs, string rhs, bool normal) {
            return operatorTemplate->printMe(lhs, rhs, normal);
        }

        void generateAdditionalVarsForExample(int exampleId, int skeletonId, int maxLength,VarsDict* dict){
            for (int timeStep = 0; timeStep < maxLength; timeStep++){
                //Alpha
                Variable* varETS = new Variable(VarType::metaETS);
                varETS->exampleId = exampleId;
                varETS->timestep = timeStep;
                varETS->skeletonId = skeletonId;
                varETS->alpha = 1;
                dict->addVariable(varETS);

                Variable* varETSN = new Variable(VarType::metaETS);
                varETSN->exampleId = exampleId;
                varETSN->timestep = timeStep;
                varETSN->skeletonId = skeletonId;
                varETSN->alpha = 2;
                dict->addVariable(varETSN);
                //BETA if Exists
                if (hasBeta) {
                    Variable* varETS1 = new Variable(VarType::metaETS);
                    varETS1->exampleId = exampleId;
                    varETS1->timestep = timeStep;
                    varETS1->skeletonId = skeletonId;
                    varETS1->alpha = 3;
                    dict->addVariable(varETS1);
                    Variable* varETS1N = new Variable(VarType::metaETS);
                    varETS1N->exampleId = exampleId;
                    varETS1N->timestep = timeStep;
                    varETS1N->skeletonId = skeletonId;
                    varETS1N->alpha = 4;
                    dict->addVariable(varETS1N);
                }
                operatorTemplate->genVariablesForExamples(exampleId,skeletonId,maxLength,dict,5);
            }

        }

        vector<vector<int> > genClausesRecursive(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict, OperatorTemplate* currentPos, int st, bool dual){
            //leafs are already covered
            vector<vector<int> > clauses;
            if (currentPos->leaf != 0) {
                return clauses;
            }
            VarPackage varPackage;
            varPackage.ets = dict.getMetaEtsId(exampleId,timeStep,skeletonId,currentPos->varId);
            varPackage.st = st;
            if (currentPos->isUnary) {
                        varPackage.alpha = dict.getMetaEtsId(exampleId, timeStep, skeletonId, currentPos->rhs->getMetaId());
                    for (vector<int> cl : genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize,dict,currentPos->rhs,st,dual)){
                        clauses.push_back(cl);
                    }
            }
            else {
                        varPackage.alpha = dict.getMetaEtsId(exampleId, timeStep, skeletonId, currentPos->lhs->getMetaId());
                        varPackage.beta = dict.getMetaEtsId(exampleId, timeStep, skeletonId, currentPos->rhs->getMetaId());
                    for (vector<int> cl : genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize,dict,currentPos->rhs,st,dual)){
                        clauses.push_back(cl);
                    }
                    for (vector<int> cl : genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize,dict,currentPos->lhs,st,dual)){
                        clauses.push_back(cl);
                    }
            } 

            if (timeStep + 1 < maxLength){
                varPackage.etsPrime= dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, currentPos->varId);
                    varPackage.alphaPrime = dict.getMetaEtsId(exampleId, timeStep + 1, skeletonId, currentPos->rhs->getMetaId());  
            }
            if (dual) {
                for (vector<int> cl : genDualClausesForTemplates(currentPos->type, timeStep,maxLength, varPackage)){
                    clauses.push_back(cl);
                }
            } else {
                for (vector<int> cl : genClausesForTemplates(currentPos->type, timeStep,maxLength, varPackage)){
                    clauses.push_back(cl);
                }
            }
            return clauses;
        }



        vector<vector<int> >genClausesUnary(int exampleId, int timeStep,int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, metaType);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                int varSTLiteralAlpha = dict.getVarSTId(alphaSkeletonId, SkeletonType::literal);
                vector<int> enforceAlpha = {-varETSId,-varSTId, varSTLiteralAlpha};
                clauses.push_back(enforceAlpha);
                    int varAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 1);
                    int negVarAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 2);
                    vector<int> alphaVal = {-varAlpha, dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId), -dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    clauses.push_back(alphaVal);

                    vector<int> negAlphaVal = {-negVarAlpha, dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId), dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    clauses.push_back(negAlphaVal);
                    int varFirstOp = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 5);
                vector<int> entryPoint = {-varETSId, varFirstOp};
                clauses.push_back(entryPoint);
                vector<vector<int> > skeletonClauses = genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize, dict, operatorTemplate, varSTId,false);
                for (vector<int> cl : skeletonClauses){
                    clauses.push_back(cl);
                }
            }
            return clauses;
        }


        virtual vector<vector<int>> genDualClausesUnary(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, metaType);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                    int varFirstOp = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 5);


                    int varAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 1);
                    int negVarAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 2);
                    vector<int> alphaVal = {-varAlpha, dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId), -dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                        clauses.push_back(alphaVal);

                    vector<int> negAlphaVal = {-negVarAlpha, dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId), dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    clauses.push_back(negAlphaVal);

                    vector<int> entryPoint = {-varETSId, varFirstOp};
                    clauses.push_back(entryPoint);
                    
                    vector<vector<int> > skeletonClauses = genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize, dict, operatorTemplate, varSTId, true);
                    for (vector<int> cl : skeletonClauses){
                        clauses.push_back(cl);
                    }
            }
            return clauses;
        }



        virtual vector<vector<int>> genClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            if (!hasBeta)
                return genClausesUnary(exampleId,timeStep, skeletonId,maxLength, fmlSize,dict);
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, metaType);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId ++){
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId ++){
            

                    //Enforce Alpha and Beta to be literals
                    int varSTLiteralAlpha = dict.getVarSTId(alphaSkeletonId, SkeletonType::literal);
                    int varSTLiteralBeta = dict.getVarSTId(betaSkeletonId, SkeletonType::literal);

                    vector<int> enforceBeta = {-varSTId, varSTLiteralBeta};
                    vector<int> enforceAlpha = {-varSTId, varSTLiteralAlpha};
                    clauses.push_back(enforceBeta);
                    clauses.push_back(enforceAlpha);
                    


                    int varAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 1);
                    vector<int> alphaVal = {-varAlpha, dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId), -dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    clauses.push_back(alphaVal);

                    int negVarAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 2);
                    vector<int> negAlphaVal = {-varSTId, -varAlpha,-negVarAlpha, -dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    vector<int> negAlphaVal2 = {-varSTId, varAlpha,negVarAlpha, -dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    clauses.push_back(negAlphaVal);
                    clauses.push_back(negAlphaVal2);

                    int varBeta = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 3);
                    vector<int> betaVal = {-varBeta, dict.getVarEtsId(exampleId, timeStep, betaSkeletonId), -dict.getVarBetaId(skeletonId, betaSkeletonId)};
                    clauses.push_back(betaVal);

                    int negVarBeta = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 4);
                    vector<int> negBetaVal = {-varSTId, -varBeta,-negVarBeta,-dict.getVarBetaId(skeletonId, betaSkeletonId)};
                    vector<int> negBetaVal2 = {-varSTId, varBeta,negVarBeta,-dict.getVarBetaId(skeletonId, betaSkeletonId)};
                    clauses.push_back(negBetaVal);
                    clauses.push_back(negBetaVal2);
                    
                    int varFirstOp = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 5);
                    vector<int> entryPoint = {-varETSId,-varSTId, varFirstOp};
                    clauses.push_back(entryPoint);
                    vector<vector<int> > skeletonClauses = genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize, dict, operatorTemplate, varSTId,false);
                    for (vector<int> cl : skeletonClauses){
                        clauses.push_back(cl);
                    }

                }
            }
            return clauses;
        }

        virtual vector<vector<int>> genDualClauses(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict) {
            if (!hasBeta)
                return genDualClausesUnary(exampleId,timeStep, skeletonId,maxLength, fmlSize,dict);
            vector<vector<int> > clauses;
            int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
            int varSTId = dict.getVarSTId(skeletonId, metaType);
            for (int alphaSkeletonId = skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++) {
                for (int betaSkeletonId = skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++) { 
                    int varFirstOp = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 5);
                    vector<int> entryPoint = {-varETSId, -varSTId, varFirstOp};
                    clauses.push_back(entryPoint);

                    int varAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 1);
                    vector<int> alphaVal = {-varAlpha, dict.getVarEtsId(exampleId, timeStep, alphaSkeletonId), -dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    clauses.push_back(alphaVal);

                    int negVarAlpha = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 2);
                    vector<int> negAlphaVal = {-varSTId, -varAlpha,-negVarAlpha,-dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    vector<int> negAlphaVal2 = {-varSTId, varAlpha,negVarAlpha,-dict.getVarAlphaId(skeletonId,alphaSkeletonId)};
                    clauses.push_back(negAlphaVal);
                    clauses.push_back(negAlphaVal2);

                    int varBeta = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 3);
                    vector<int> betaVal = {-varBeta, dict.getVarEtsId(exampleId, timeStep, betaSkeletonId), -dict.getVarBetaId(skeletonId, betaSkeletonId)};
                    clauses.push_back(betaVal);

                    int negVarBeta = dict.getMetaEtsId(exampleId,timeStep, skeletonId, 4);
                    vector<int> negBetaVal = {-varSTId, -varBeta,-negVarBeta,-dict.getVarBetaId(skeletonId, betaSkeletonId)};
                    vector<int> negBetaVal2 = {-varSTId, varBeta,negVarBeta,-dict.getVarBetaId(skeletonId, betaSkeletonId)};
                    clauses.push_back(negBetaVal);
                    clauses.push_back(negBetaVal2);

                    vector<vector<int> > skeletonClauses = genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize, dict, operatorTemplate, varSTId,true);
                    for (vector<int> cl : skeletonClauses){
                        clauses.push_back(cl);
                    }


                }
            }
            return clauses;
        }

        public: 
        MetaOperator(int type, bool isBinary, OperatorTemplate* operatorTemplate, int varsPerExample):metaType(type),hasBeta(isBinary),operatorTemplate(operatorTemplate) {
            this->addVarsPExample = varsPerExample;
            this->isMeta = true;
}
};


