#include "../operator.hpp"
#include "operatorTemplate.hpp"

/**
 *  Generatic MetaOperator Implementation 
 *
 */
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


        /**
         *  Prints MetaOperator according to the Metaoperatorskeleton
         *
         */ 
        string printUnaryOperator(string lhs, bool normal) {
            return operatorTemplate->printMe(lhs, "ERROR", normal);
        }

        /**
         *  Prints MetaOperator according to the Metaoperatorskeleton
         *
         */ 
        string printBinaryperator(string lhs, string rhs, bool normal) {
            return operatorTemplate->printMe(lhs, rhs, normal);
        }

        /**
         *  Metaoperators can be learned for a formula size of 3, so we need to add additional variables for the inner tree.
         *  Example :  our meta operator is G ||, we say G || is equal to GAnd, so our CNF generates only variables for GAnd , a and b, 
         *  since the tree looks like this          
         *          GA
         *        /    \
         *       a      b
         *    But our inner tree has still
         *
         *          G
         *          |
         *         AND
         *  So we need additional RunVariables for the inner nodes.
         *
         *  @param exampleId - the current Plan
         *  @param skeletonId - the current skeletonNode
         *  @param maxLength - the length of the Plan
         *  @param dict - the VariableDictionary to get information of skeletons and literals
         */
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
        

        /**
         *  
         *  Creates the clauses for the metaoperator, 
         *
         *  Can be split in multiple cases 
         *      1. We have a leaf, means there may only be literals -> nothing to be done, since we covered this in literal type clause generation
         *      2. We have an unary operator, so we generate the clauses for the unary operator and call this for the subformula(alpha)
         *      3. We have a binary operator, so we generate the clauses for the binary operator and call this for both subformulas(alpha nad beta)
         *  
         *  @params exampleId - the id of the current example
         *  @params timeStep - the current timestep
         *  @params skeletonId - the current skeletonId (should be 0), since we currently dont allow chains of metaoperators
         *  @params maxLength - the number of states in the plan
         *  @param fmlSize - the maximal formula size in number of possible nodes
         *  @param dict - the variable dictionary to get information for variables ,etc/
         *  @param currentPos - the pointer to the current position of inside the tree structure of the operator template
         *  @param outerSkeletonType - the variable of the outerskeleton type to refer the clauses to this operator
         *  @param dual - wether to generate clauses for positive and negative examples.
         */ 
        vector<vector<int> > genClausesRecursive(int exampleId, int timeStep, int skeletonId, int maxLength, int fmlSize, VarsDict dict, OperatorTemplate* currentPos, int outerSkeletonType, bool dual){
            // first case : leafs are already covered
            vector<vector<int> > clauses;
            if (currentPos->leaf != 0) {
                return clauses;
            }
            VarPackage varPackage;
            varPackage.ets = dict.getMetaEtsId(exampleId,timeStep,skeletonId,currentPos->varId);
            varPackage.st = outerSkeletonType;
            //second case : unary
            if (currentPos->isUnary) {
                        varPackage.alpha = dict.getMetaEtsId(exampleId, timeStep, skeletonId, currentPos->rhs->getMetaId());
                    for (vector<int> cl : genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize,dict,currentPos->rhs,outerSkeletonType,dual)){
                        clauses.push_back(cl);
                    }
            }
            // third case : binary
            else {
                        varPackage.alpha = dict.getMetaEtsId(exampleId, timeStep, skeletonId, currentPos->lhs->getMetaId());
                        varPackage.beta = dict.getMetaEtsId(exampleId, timeStep, skeletonId, currentPos->rhs->getMetaId());
                    for (vector<int> cl : genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize,dict,currentPos->rhs,outerSkeletonType,dual)){
                        clauses.push_back(cl);
                    }
                    for (vector<int> cl : genClausesRecursive(exampleId, timeStep, skeletonId, maxLength, fmlSize,dict,currentPos->lhs,outerSkeletonType,dual)){
                        clauses.push_back(cl);
                    }
            } 

            // create clauses for operator types
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



        /**
         *  
         *  Creates the clauses for the unary operator runs,
         *  implement the chaining between the operator and subformula and connects them to the outer metaoperator
         *
         *  
         *  @params exampleId - the id of the current example
         *  @params timeStep - the current timestep
         *  @params skeletonId - the current skeletonId (should be 0), since we currently dont allow chains of metaoperators
         *  @params maxLength - the number of states in the plan
         *  @param fmlSize - the maximal formula size in number of possible nodes
         *  @param dict - the variable dictionary to get information for variables ,etc/
         */ 
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


        /**
         *  
         *  Creates the dual clauses for the unary operator runs,
         *  implement the chaining between the operator and subformula and connects them to the outer metaoperator
         *
         *  
         *  @params exampleId - the id of the current example
         *  @params timeStep - the current timestep
         *  @params skeletonId - the current skeletonId (should be 0), since we currently dont allow chains of metaoperators
         *  @params maxLength - the number of states in the plan
         *  @param fmlSize - the maximal formula size in number of possible nodes
         *  @param dict - the variable dictionary to get information for variables ,etc/
         */ 
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



        /**
         *  
         *  Creates the clauses for the operator runs,
         *  implement the chaining between the operator and subformula and connects them to the outer metaoperator
         *
         *  
         *  @params exampleId - the id of the current example
         *  @params timeStep - the current timestep
         *  @params skeletonId - the current skeletonId (should be 0), since we currently dont allow chains of metaoperators
         *  @params maxLength - the number of states in the plan
         *  @param fmlSize - the maximal formula size in number of possible nodes
         *  @param dict - the variable dictionary to get information for variables ,etc/
         */ 
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

        /**
         *  
         *  Creates the clauses for the operator runs,
         *  implement the chaining between the operator and subformula and connects them to the outer metaoperator
         *
         *  
         *  @params exampleId - the id of the current example
         *  @params timeStep - the current timestep
         *  @params skeletonId - the current skeletonId (should be 0), since we currently dont allow chains of metaoperators
         *  @params maxLength - the number of states in the plan
         *  @param fmlSize - the maximal formula size in number of possible nodes
         *  @param dict - the variable dictionary to get information for variables ,etc/
         */ 
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

        /**
         *
         * Constructor for the Metaoperator
         *
         * @param type - the id of the type, usually starting at 10 
         * @param isBinary - if this operator is something like  F G a (unary) or G || a b (binary)
         * @param operatorTemplate - the inner structure oft the metaOperator
         * @param varsPerExample - the number of variables needed to track the runs in the inner tree
         *
         */
        MetaOperator(int type, bool isBinary, OperatorTemplate* operatorTemplate, int varsPerExample):metaType(type),hasBeta(isBinary),operatorTemplate(operatorTemplate) {
            this->addVarsPExample = varsPerExample;
            this->isMeta = true;
}
};


