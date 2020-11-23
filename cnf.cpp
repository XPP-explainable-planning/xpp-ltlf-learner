#include "cnf.hpp"

#include <string>
#include <iostream>

    
    void Cnf::printStatistics() {
        cout << "############# Statistics #############" << endl;
        cout << "Num Variables after Init: " << numVariablesAfterInit << endl;
        cout << "Num Clauses after Init: " << numClausesAfterInit << endl;
        cout << endl;
        cout << "Num Variables: " << this->dict.lookupTable.size() << endl;
        cout << "Num Clauses: " << this->clauses.size() << endl;
        cout << endl;

        cout << "Learned Clauses: " << this->clauses.size() - numClausesBeforeLearning << endl;
        int traceVariables = this->dict.lookupTable.size() - numVariablesAfterInit;
        float variablesPerTrace = traceVariables / this->numExamples;
    
        cout << "Num Variables per Trace: " << variablesPerTrace << endl;

        //int traceClauses = this->numClausesBeforeLearning - numVariablesAfterInit;
        //float clausesPerTrace = traceClauses / this->numExamples;
        //cout << curSkeletonType << endl;

       // cout << "Num Variables per Trace: " << clausesPerTrace << endl;
   //     cout << this->vocabulary.size() << " & " << this->fmlSize-1 << " & " << this->traceLength << " & " << numVariablesAfterInit << " & " << numClausesAfterInit << " & " << variablesPerTrace << " & " //<< clausesPerTrace << "\\\\" <<endl;
        cout << "######################################" << endl;
    }
    
    void Cnf::printTable() {


        this->numClausesBeforeLearning = this->clauses.size();
        int traceVariables = this->dict.lookupTable.size() - numVariablesAfterInit;
        float variablesPerTrace = traceVariables / this->numExamples;
        int traceClauses = this->numClausesBeforeLearning - numVariablesAfterInit;
        float clausesPerTrace = traceClauses / this->numExamples;
        cout << this->dict.vocabulary.size() << " & " << this->fmlSize-1 << " & " << this->traceLength << " & " << numVariablesAfterInit << " & " << numClausesAfterInit << " & " << variablesPerTrace << " & " << clausesPerTrace << "\\\\" <<endl;


        }

    Cnf::Cnf(int fmlSize, vector<string> vocabulary, vector<Operator*> metaOperators, bool normal){
        initOperators(metaOperators);
        this->fmlSize = fmlSize;
        this->numExamples = 0;
        this->dict = VarsDict();
        this->dict.vocabulary = vocabulary;
        this->genInitialVariables();
        this->genInitialClauses();
        this->numClausesAfterInit = this->clauses.size();
        this->numVariablesAfterInit = this->dict.lookupTable.size();
        this->printNormal = normal;
    }
    
    void Cnf::initOperators(vector<Operator*> metaOperators){
        AllSkeletonTypes.push_back(SkeletonType::literal);
        if (!metaOperators.empty()) {
            int maxVar = 6;
            for (Operator* op : metaOperators){
                AllSkeletonTypes.push_back(op->getOperator());
                operators.push_back(op);
                maxVar = maxVar > op->addVarsPExample ? maxVar : op->addVarsPExample;
            }
            this->additionalVarPerExample = maxVar;
            return;
        }
        operators.push_back(createOperator(SkeletonType::always));
        AllSkeletonTypes.push_back(SkeletonType::always);
       // 
       operators.push_back(createOperator(SkeletonType::eventually));
        AllSkeletonTypes.push_back(SkeletonType::eventually);
       //// 
        operators.push_back(createOperator(SkeletonType::land));
         AllSkeletonTypes.push_back(SkeletonType::land);

       operators.push_back(createOperator(SkeletonType::until));
        AllSkeletonTypes.push_back(SkeletonType::until);
       //
        operators.push_back(createOperator(SkeletonType::release));
        AllSkeletonTypes.push_back(SkeletonType::release);
       // 
       operators.push_back(createOperator(SkeletonType::lor));
       AllSkeletonTypes.push_back(SkeletonType::lor);
       // 
       // 
        operators.push_back(createOperator(SkeletonType::next));
        AllSkeletonTypes.push_back(SkeletonType::next);
        
       //operators.push_back(createOperator(SkeletonType::weaknext));
       //AllSkeletonTypes.push_back(SkeletonType::weaknext);
      
       //operators.push_back(createOperator(SkeletonType::globallyAnd));
       //AllSkeletonTypes.push_back(SkeletonType::globallyAnd);
       
       //operators.push_back(createOperator(SkeletonType::sequencedEventually));
       //AllSkeletonTypes.push_back(SkeletonType::sequencedEventually);
         //AllSkeletonTypes.push_back(10);
        // operators.push_back(createOperator(10));
       //
       //operators.push_back(createOperator(SkeletonType::orderedSequenced));
       //AllSkeletonTypes.push_back(SkeletonType::orderedSequenced);
    }

    string Cnf::negatePredicate(string s){
        return "! " + s;
    }
    
    void printClause(vector<int> clause) {
            /*cout << "[";
            for (int v : clause){ 
                if (v >= 0)
                    cout << v/2 + 1 << ", ";
                else 
                    cout << v/2 - 1<<", "; 
            }
            cout<<"]" << endl;*/
        }
    
    Operator* Cnf::getOperatorForType(int type){
        for (Operator* op :operators){
            if (op->getOperator() == type){
                return op;
            }
        }
        assert(false);
    }

    void Cnf::setOperators(vector<SkeletonType> ops) {
    }

    string Cnf::recoverFormulafromModel(vector<int> model){
        int curSkeletonId = 0;
        string formula = skeletonToFormula(model, curSkeletonId);
        return formula; 
    }

    string Cnf::skeletonToFormula(vector<int> model, int curSkeletonId){
        int curSkeletonType = getSkeletonType(model, curSkeletonId);
        
        if (curSkeletonType == SkeletonType::literal){
            string literal = getLiteral(model, curSkeletonId);
            return literal;
        }

        Operator* op = getOperatorForType(curSkeletonType);
        if (op->isBinary()){
            int lhsAlphaId = getAlphaId(model, curSkeletonId);
            int rhsBetaId = getBetaId(model, curSkeletonId);
            string lhsFormula = skeletonToFormula(model, lhsAlphaId);
            string rhsFormula = skeletonToFormula(model, rhsBetaId);
            return op->printBinaryperator(lhsFormula, rhsFormula, this->printNormal);
        } else {
            int lhsAlphaId = getAlphaId(model, curSkeletonId);
            string lhsFormula = skeletonToFormula(model, lhsAlphaId);
            return op->printUnaryOperator(lhsFormula, this->printNormal);
        }

        return "NO Type found";
    }

    int Cnf::getSkeletonType(vector<int> model, int curSkeletonId){
        for (const auto skeletonType : AllSkeletonTypes) {
            int varStId = dict.getVarSTId(curSkeletonId, skeletonType);
            vector<int>::iterator it = find (model.begin(), model.end(), varStId);
            if (it != model.end()){
                learnClause.push_back(-varStId);
                return skeletonType;
            }
        }      
        assert(false);
    }

    int Cnf::getAlphaId(vector<int> model, int skeletonId){
        for (int alphaSkeletonId= skeletonId + 1; alphaSkeletonId < fmlSize; alphaSkeletonId++){
            int alphaId = dict.getVarAlphaId(skeletonId, alphaSkeletonId);
            vector<int>::iterator it = find (model.begin(), model.end(), alphaId);
            if (it != model.end()){
                learnClause.push_back(-alphaId);
                return alphaSkeletonId;
            }
        }      
        assert(false);       
    }


    int Cnf::getBetaId(vector<int> model, int skeletonId){
        for (int betaSkeletonId= skeletonId + 2; betaSkeletonId < fmlSize; betaSkeletonId++){
            int betaId = dict.getVarBetaId(skeletonId, betaSkeletonId);
            vector<int>::iterator it = find (model.begin(), model.end(), betaId);
            if (it != model.end()){
                learnClause.push_back(-betaId);
                return betaSkeletonId;
            }
        }      
        assert(false);       
    }

    string Cnf::getLiteral(vector<int> model, int skeletonId){
        for (int i = 0; i < this->dict.vocabulary.size();i++) {
            int literal = i + 1;
            int varLiteralId = dict.getVarLitId(skeletonId, literal);
            vector<int>::iterator it = find (model.begin(), model.end(), varLiteralId);
            if (it != model.end()){
                learnClause.push_back(-varLiteralId);
                return this->dict.vocabulary[i];
            }
            literal = -(i + 1);
            varLiteralId = dict.getVarLitId(skeletonId, literal);
            it = find (model.begin(), model.end(), varLiteralId);
            if (it != model.end()){
                learnClause.push_back(-varLiteralId);
                return negatePredicate(this->dict.vocabulary[i]);
            }
        }      
        assert(false); 
    }
    
    


    void Cnf::addPositiveExample(vector<vector<string> > positiveExample){
        this->traceLength = positiveExample.size();
        genVariablesForExample(positiveExample);

        genSkeletonClausesForPosExample(positiveExample);
        numExamples++;
    }

    void Cnf::addNegativeExample(vector<vector<string> > negativeExample){
        this->traceLength = negativeExample.size();
        genVariablesForExample(negativeExample);

        genSkeletonClausesForNegExample(negativeExample);
        numExamples++;
    }


    void Cnf::genInitialVariables(){
        // these formulae are var(s,type) where 
        // s is a skeleton_id and
        // type is a skeleton type
        for (int skeletonId = 0; skeletonId < fmlSize; skeletonId++){
            for (const auto skeletonType : AllSkeletonTypes){
                Variable* var = new Variable(VarType::ST);
                var->skeletonId = skeletonId;
                var->skeletonType = skeletonType;
                dict.addVariable(var);
            }
        }
         /*these formulae are varAlpha(s,s') and varBeta(s,s'),
         where s,s' are skeleton_ids
         in order to avoid loops, we only consider 
         skeletonAlpha_id > skeleton_id
         skeletonBeta_id > skeletonAlpha_id*/

        for (int skeletonId = 0; skeletonId < fmlSize; skeletonId++){
            for (int skeletonAlphaId = skeletonId + 1; skeletonAlphaId < fmlSize; skeletonAlphaId++ ){
                Variable* var = new Variable(VarType::alpha);
                var->skeletonId = skeletonId;
                var->alpha = skeletonAlphaId;
                dict.addVariable(var);
            }
            for (int skeletonBetaId = skeletonId + 2; skeletonBetaId < fmlSize; skeletonBetaId++ ){
                Variable* var = new Variable(VarType::beta);
                var->skeletonId = skeletonId;
                var->beta = skeletonBetaId;
                dict.addVariable(var);
            }
        }
        
        for (int skeletonId = 0; skeletonId < fmlSize; skeletonId++){
            for (int varCount = 0; varCount < this->dict.vocabulary.size(); varCount++){
                Variable* var = new Variable(VarType::skliteral);
                var->skeletonId = skeletonId;
                var->literal = varCount + 1;
                dict.addVariable(var);

                Variable* negVar = new Variable(VarType::skliteral);
                negVar->skeletonId = skeletonId;
                negVar->literal = - (varCount + 1);
                dict.addVariable(negVar);
            }
            for (Operator* op : operators) {
                op->generateAdditionalVars(skeletonId,&dict);
            }
        }


        numVariablesAfterInit = dict.lookupTable.size();
    }

    string Cnf::learnFormula() {
        string s ="";
        Minisat::Solver solver = Solver();
        for (int i = 0; i < dict.lookupTable.size(); i ++){
            solver.newVar();
        }

        for (vector<int> clause : clauses) { 
            Minisat::vec<Minisat::Lit> clauseLit; 
            for (int var : clause) {
                if (var < 0) {
                    clauseLit.push(toLit((- var)));
                } else { 
                    clauseLit.push(toLit(var - 1));
                }
            }
            solver.addClause(clauseLit);
        }
        if (solver.solve()) {
            vector<int> model;
            for (auto val :  solver.model ) {
                if (val.isTrue()){
                    model.push_back(2* model.size() + 1);
                } else {
                    model.push_back(- (2* model.size() + 1));
                }
            }
            learnClause.clear();
            s = recoverFormulafromModel(model);
            clauses.push_back(learnClause);

        }
        return s;
    }

    vector<string> Cnf::learnFormulas(int formulaCount){
        bool infinite = formulaCount == -1;
        int count = 0;
        this->numClausesBeforeLearning = this->clauses.size();
        vector<string> formulas;
        string formula = learnFormula();
        
       // for (vector<int> v : this->clauses){
       //     for (int i : v) {
       //         cout << i << " ";
       //     }
       //     cout << endl;
       // }
        cout << "Num Clauses: " << this->clauses.size() << "  Variables: " << this->dict.lookupTable.size() << endl;
        while ((formula.compare("") != 0 && (infinite || count < formulaCount))){
            cout << "learned Fromulas: " << count << endl;
            formulas.push_back(formula);
            formula = learnFormula();
            count ++;
        }
        return formulas;
    }


    void Cnf::atMostOne(vector<int> ids){
        for (int i = 0; i < ids.size(); i++) {
            for (int j = i + 1; j < ids.size(); j++){
                vector<int> newClause  {- ids.at(i), - ids.at(j)};
                printClause(newClause);
                clauses.push_back(newClause);
            }
        }
    }

    void Cnf::exactlyOne(vector<int> ids){
        atMostOne(ids);
        printClause(ids);
        clauses.push_back(ids);
    }

    void  Cnf::genInitialClauses() {
        //oneof (s, type)
        for (int skeletonId = 0; skeletonId < fmlSize; skeletonId ++){
            vector<int> stIds;
            for (int skType : AllSkeletonTypes) {
               int varId = dict.getVarSTId(skeletonId, skType);
               stIds.push_back(varId);
            }
            exactlyOne(stIds);
       } 
        //one of varAlpha(s,s')
        for (int skeletonId = 0; skeletonId < fmlSize; skeletonId ++){
            vector<int> alphaIds;
            for (int alphaId = skeletonId + 1; alphaId < fmlSize; alphaId ++) {
                int varAlphaId = dict.getVarAlphaId(skeletonId, alphaId);
                alphaIds.push_back(varAlphaId);
            }
            if (!alphaIds.empty()) {
                for (int a : alphaIds){
                } 
                exactlyOne(alphaIds);
            }
       } 

        for (int skeletonId = 0; skeletonId < fmlSize; skeletonId ++) {
            vector<int> betaIds;
            for (int skBetaId = skeletonId + 2; skBetaId < fmlSize; skBetaId++) {
                int betaId = dict.getVarBetaId(skeletonId, skBetaId);
                betaIds.push_back(betaId);
            }
            if (!betaIds.empty()){
                exactlyOne(betaIds);
            }
        }

        //atmost one varLit (s,p)
        for (int skeletonId = 0; skeletonId < fmlSize; skeletonId++){
            vector<int> litIds;
            for (int p = 0; p < this->dict.vocabulary.size(); p++) {
                int literal = p + 1;
                int varLitId = dict.getVarLitId(skeletonId, literal);
                litIds.push_back(varLitId);
                literal = - literal;
                varLitId = dict.getVarLitId(skeletonId, literal);
                litIds.push_back(varLitId);
            }
            atMostOne(litIds);
            int varSTId = dict.getVarSTId(skeletonId, SkeletonType::literal);
            litIds.push_back(-varSTId);
            printClause(litIds);
            clauses.push_back(litIds);
        }

        // force to not have until, release, and, or  with skeletonid >= fml_len - 2
        //
        for (Operator* op: operators){
            for (int skeletonId = fmlSize - op->getNumSkeletons(); skeletonId < fmlSize; skeletonId++) {
               vector<int> varST;
               varST.push_back(- dict.getVarSTId(skeletonId, op->getOperator()));
               printClause(varST);
               clauses.push_back(varST);
            }
        }

        //force to have only metaOperators at skeletonId 0
        for (Operator* op: operators){
            for (int skeletonId = 1; skeletonId < fmlSize; skeletonId++) {
                if (op->isMeta) {
                    vector<int> varST;
                    varST.push_back(- dict.getVarSTId(skeletonId, op->getOperator()));
                    printClause(varST);
                    clauses.push_back(varST);
                }           
            }
        }

        /*for (int skeletonId = fmlSize -2; skeletonId < fmlSize; skeletonId++){
            for (Operator* op : operators){
                if (op->isBinary()){
                    vector<int> varST;
                    varST.push_back(- dict.getVarSTId(skeletonId, op->getOperator()));
                    printClause(varST);
                    clauses.push_back(varST);
                }
            }
        }

        //force not to have next and weaknext, globally, eventually operators with skeleton_id >= fml_len - 1
        for (int skeletonId = fmlSize -1; skeletonId < fmlSize; skeletonId++) {
            for (Operator* op : operators){
                if (!op->isBinary()){
                    vector<int> varST;
                    varST.push_back(- dict.getVarSTId(skeletonId, op->getOperator()));
                    printClause(varST);
                    clauses.push_back(varST);
                }
            }
        }*/
    }



    void Cnf::genVariablesForExample(vector<vector<string> > example) {
        int exampleId = numExamples;
         for (int timeStep = 0; timeStep < example.size(); timeStep++) {
            for (int skeletonId = 0; skeletonId < fmlSize; skeletonId++) {
                Variable* var = new Variable(VarType::ETS);
                var->exampleId = exampleId;
                var->timestep = timeStep;
                var->skeletonId = skeletonId;
                dict.addVariable(var);
            }
         }
        for (int skeletonId = 0; skeletonId < 1; skeletonId++) {
            for (int timeStep = 0; timeStep < example.size(); timeStep++){
                for (int pos = 1; pos < this->additionalVarPerExample; pos++) {
                    Variable* varETS = new Variable(VarType::metaETS);
                    varETS->exampleId = exampleId;
                    varETS->timestep = timeStep;
                    varETS->skeletonId = skeletonId;
                    varETS->alpha = pos;
                    dict.addVariable(varETS);
                }   
            }
       }
      /*for (int skeletonId = 0; skeletonId < fmlSize; skeletonId++) {
            for (Operator* op : operators) {
                op->generateAdditionalVarsForExample(exampleId, skeletonId, example.size(), &dict);
            }
         }*/
    }

    void Cnf::genSkeletonClausesForPosExample(vector<vector<string> > example){
        int exampleId = numExamples;
        vector<int> varETSId;
        varETSId.push_back(dict.getVarEtsId(exampleId, 0, 0));
        printClause(varETSId);
        clauses.push_back(varETSId);
        int exampleLength = example.size();
        for (int timeStep = 0; timeStep < example.size(); timeStep++) {
            for (int skeletonId = 0;  skeletonId < fmlSize; skeletonId++) {
                vector<string> observation = example[timeStep];
                for (Operator* op : operators){
                    if ( !op->isMeta || (skeletonId == 0)) {
                        vector<vector<int> > opClauses = op->genClauses(exampleId,timeStep,skeletonId,exampleLength, fmlSize, dict);
                        for (vector<int> cl : opClauses){
                            clauses.push_back(cl);
                        }
                    }
                }
                genClausesLiteral(exampleId, timeStep, skeletonId, observation);
            }
        }


    }


    void Cnf::genSkeletonClausesForNegExample(vector<vector<string> > example){
        int exampleId = numExamples;
        vector<int> varETSId;
        varETSId.push_back(dict.getVarEtsId(exampleId, 0, 0));
        printClause(varETSId);
        clauses.push_back(varETSId);
        int exampleLength = example.size();
        
        for (int timeStep = 0; timeStep < example.size(); timeStep++) {
            for (int skeletonId = 0;  skeletonId < fmlSize; skeletonId++) {
                for (Operator* op : operators){
                    if ( !op->isMeta || (skeletonId == 0)) {
                        vector<vector<int> > opClauses = op->genDualClauses(exampleId,timeStep,skeletonId,exampleLength, fmlSize, dict);
                        for (vector<int> cl : opClauses){
                            clauses.push_back(cl);
                        }
                    }
                }
                vector<string> observation = example[timeStep];
                genDualClausesLiteral(exampleId, timeStep, skeletonId, observation);

            }
        }
    }

    void Cnf::genClausesLiteral(int exampleId, int timeStep, int skeletonId, vector<string> observation) {
        int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
        int varSTId = dict.getVarSTId(skeletonId, SkeletonType::literal);

        for (int p = 0; p < this->dict.vocabulary.size(); p++) {
            int literal = p + 1;
            int varLitId = dict.getVarLitId(skeletonId, literal);
            int varNegLitId = dict.getVarLitId(skeletonId, -literal);
            bool found = false;
            for(string ap :  observation){
                if (ap.compare(this->dict.vocabulary[p]) == 0){
                    found = true; 
                }
            }
    
            vector<int> clause;
            clause.push_back(-varETSId);
            clause.push_back(-varSTId);
            if (!found) {
                clause.push_back(-varLitId);
            } else {
                clause.push_back(-varNegLitId);
            }
            printClause(clause);
            clauses.push_back(clause);
        }
    }


    void Cnf::genDualClausesLiteral(int exampleId, int timeStep, int skeletonId, vector<string> observation) {
        int varETSId = dict.getVarEtsId(exampleId, timeStep, skeletonId);
        int varSTId = dict.getVarSTId(skeletonId, SkeletonType::literal);

        for (int p = 0; p < this->dict.vocabulary.size(); p++) {
            int literal = p + 1;
            int varLitId = dict.getVarLitId(skeletonId, literal);
            int varNegLitId = dict.getVarLitId(skeletonId, -literal);
            bool found = false;
            for(string ap :  observation){
                if (ap.compare(this->dict.vocabulary[p]) == 0){
                    found = true; 
                }
            }
    
            vector<int> clause;
            clause.push_back(-varETSId);
            clause.push_back(-varSTId);
            if (found) {
                clause.push_back(-varLitId);
            } else {
                clause.push_back(-varNegLitId);
            }
            printClause(clause);
            clauses.push_back(clause);
        }
    }
