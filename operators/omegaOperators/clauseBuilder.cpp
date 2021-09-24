#include "operatorTemplate.hpp"
#include <vector>

using namespace std;

/**
 *  The Clause for Next, similar to the clause generation in the NextOperator Class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genClausesTemplateNext(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep == maxLength - 1) {
            vector<int> clause  {-vars.ets, -vars.st};
            clauses.push_back(clause);
        } else {
            vector<int> clause  {-vars.ets, -vars.st, vars.alphaPrime};
            clauses.push_back(clause);
        }
        return clauses;
    }


/**
 *  The DualClause for Next, similar to the clause generation in the NextOperator Class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genDualClausesTemplateNext(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep + 1 < maxLength){
            return genClausesTemplateNext(timeStep,maxLength,vars);
        }
        return clauses;
    }


/**
 *  The clause generation for an eventually template, similar to the clause generation in the eventually operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genClausesTemplateEventually(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep == maxLength - 1) {
            vector<int> clause = {-vars.ets, -vars.st, vars.alpha};
            clauses.push_back(clause);
        } else {
            vector<int> clause = {-vars.ets, -vars.st, vars.alpha, vars.etsPrime};
            clauses.push_back(clause);
        }
        return clauses;
    }


/**
 *  The dual clause generation for an eventually template, similar to the clause generation in the eventually operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genDualClausesTemplateEventually(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        vector<int> clause = {-vars.ets, -vars.st, vars.alpha};
        clauses.push_back(clause);
        if (timeStep + 1 < maxLength ){
            vector<int> clause2 = {-vars.ets, -vars.st, vars.etsPrime};
            clauses.push_back(clause2);
        }
        return clauses;
    }


/**
 *  The clause generation for an globally template, similar to the clause generation in the globally operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genClausesTemplateAlways(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        vector<int> clause  {-vars.ets, -vars.st, vars.alpha};
        clauses.push_back(clause);
        if (timeStep + 1 < maxLength){ 
            vector<int> cornerCase  {-vars.ets, -vars.st, vars.etsPrime};        
            clauses.push_back(cornerCase);
        }
        return clauses;
    }

/**
 *  The dual clause generation for an globally template, similar to the clause generation in the globally operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genDualClausesTemplateAlways(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep == maxLength - 1) {
            vector<int> cl = {-vars.ets, -vars.st, vars.alpha};
            clauses.push_back(cl);
        } else {
            vector<int> cornerCase  {-vars.ets, -vars.st, vars.alpha, vars.etsPrime};
            clauses.push_back(cornerCase);
        }
        return clauses;
    }


/**
 *  The clause generation for an and template, similar to the clause generation in the and-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genClausesTemplateAnd(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        vector<int> clause1  {-vars.ets, -vars.st, vars.alpha};
        vector<int> clause2  {-vars.ets, -vars.st, vars.beta};
        clauses.push_back(clause1);
        clauses.push_back(clause2);
        return clauses;
    }

/**
 *  The dual clause generation for an and template, similar to the clause generation in the and-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genDualClausesTemplateAnd(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        vector<int> clause1  {-vars.ets, -vars.st, vars.alpha, vars.beta};
        clauses.push_back(clause1);
        return clauses;
    }


/**
 *  The clause generation for an or-template, similar to the clause generation in the or-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genClausesTemplateOr(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        vector<int> clause  {-vars.ets, -vars.st, vars.alpha, vars.beta};
        clauses.push_back(clause);
        return clauses;
    }

/**
 *  The dual clause generation for an or-template, similar to the clause generation in the or-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 *
 *
 */ 
    vector<vector<int> > genDualClausesTemplateOr(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        vector<int> clause1  {-vars.ets, -vars.st, vars.alpha};
        vector<int> clause2  {-vars.ets, -vars.st, vars.beta};
        clauses.push_back(clause1);
        clauses.push_back(clause2);
        return clauses;
    }


/**
 *  The clause generation for a release-template, similar to the clause generation in the release-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 */ 
    vector<vector<int> > genClausesTemplateRelease(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep + 1 < maxLength) {
            vector<int> clauseBeta  {-vars.ets, -vars.st,vars.beta};
            clauses.push_back(clauseBeta);

            vector<int> clauseReleaseOrAlpha   {-vars.ets, -vars.st, vars.alpha, vars.etsPrime};
            clauses.push_back(clauseReleaseOrAlpha);
        } 
        else if (timeStep == maxLength -1) {
            vector<int> clauseBeta  {-vars.ets, -vars.ets, vars.beta};
            clauses.push_back(clauseBeta);

        }
        return clauses;
    }


/**
 *  The dual clause generation for a release-template, similar to the clause generation in the release-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 */ 
    vector<vector<int> > genDualClausesTemplateRelease(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep + 1 < maxLength) {

            // either ((aUb)' and a) or b \equiv ((aUb)' or b) and (a or b)
            vector<int> clauseBeta  {-vars.ets, -vars.st, vars.alpha, vars.beta};
            clauses.push_back(clauseBeta);
            vector<int> clauseReleaseOrAlpha   {- vars.ets, -vars.st, vars.beta , vars.etsPrime};
            clauses.push_back(clauseReleaseOrAlpha);
        } 
        else if (timeStep == maxLength -1) {
            vector<int> clauseBeta  {-vars.ets, -vars.st, vars.beta};
            clauses.push_back(clauseBeta);
        }

        return clauses;
    }


/**
 *  The clause generation for an until-template, similar to the clause generation in the until-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 */ 
    vector<vector<int> > genClausesTemplateUntil(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep + 1 < maxLength) {
            vector<int> clauseAoB  {-vars.ets, -vars.st, vars.alpha, vars.beta};
            clauses.push_back(clauseAoB);
            vector<int> clauseUntilPrimeOB  {-vars.ets, -vars.st, vars.beta, vars.etsPrime};
            clauses.push_back(clauseUntilPrimeOB);
        } else if (timeStep + 1 == maxLength) {
            vector<int> clauseBeta  {-vars.ets, -vars.st, vars.beta};
            clauses.push_back(clauseBeta);
        }
        
        return clauses;
    }


/**
 *  The dual clause generation for an until-template, similar to the clause generation in the until-operator class
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 */ 
    vector<vector<int> > genDualClausesTemplateUntil(int timeStep, int maxLength, VarPackage vars){
        vector<vector<int> > clauses;
        if (timeStep + 1 < maxLength) {
            //either ((aRb)' and b) or (a and b) \equiv b and ( a or (aRb)' )
            vector<int> clauseBeta  {-vars.ets, -vars.st, vars.beta};
            clauses.push_back(clauseBeta);
            vector<int> clauseReleasePrimeOrAlpha = {-vars.ets, -vars.st, vars.alpha, vars.etsPrime};
            clauses.push_back(clauseReleasePrimeOrAlpha);

        } else if (timeStep + 1 == maxLength) {
            vector<int> clauseBeta  {-vars.ets, -vars.st, vars.beta};
            clauses.push_back(clauseBeta);
        }
        return clauses;
    }


/**
 *  The clause generation for an operator template at a specific position,
 *  switches over the skeleton type and then calls the skeletontype-tailored function
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 */ 
    vector<vector<int> > genClausesForTemplates(SkeletonType type,int timeStep, int maxLength, VarPackage vars) {
        switch(type) {
               
            case SkeletonType::next: {
                return genClausesTemplateNext(timeStep, maxLength, vars);
            }    
            case SkeletonType::eventually: {
                return genClausesTemplateEventually(timeStep, maxLength, vars);
            }    
            case SkeletonType::always: {
                return genClausesTemplateAlways(timeStep, maxLength, vars);
            }    
            case SkeletonType::land: {
                return genClausesTemplateAnd(timeStep, maxLength, vars);
            }    
            case SkeletonType::lor: {
                return genClausesTemplateOr(timeStep, maxLength, vars);
            }    
            case SkeletonType::release: {
                return genClausesTemplateRelease(timeStep, maxLength, vars);
            }    
            case SkeletonType::until: {
                return genClausesTemplateUntil(timeStep, maxLength, vars);
            }    
            default: {
                assert(false);
            }

        }
        assert(false);
   }


/**
 *  The dual clause generation for an operator template at a specific position,
 *  switches over the skeleton type and then calls the skeletontype-tailored function
 *
 *  @param timeStep - the current timestep of the plan
 *  @param maxLength - th number of steps in the plan
 *  @param vars - a struct containing all important variable ids such as 
 *      - ets - the current evaluation of the inner node 
 *      - st - the skeleton type variable for the metaoperator at the current skeleton id position
 *      - alpha - the variable refering to the run of the alphasubformula of the current timestep
 *      - beta - the variable refering to the run of the betasubformula of the current timestep
 *      - etsprime - the variable refering to the evaluation in the next timestep
 *      - alphaprime - the variable refering to the evaluation of alpha in the next timestep
 *
 */ 
    vector<vector<int> > genDualClausesForTemplates(SkeletonType type,int timeStep, int maxLength, VarPackage vars) {
        switch(type) {
               
            case SkeletonType::next: {
                return genDualClausesTemplateNext(timeStep, maxLength, vars);
            }    
            case SkeletonType::eventually: {
                return genDualClausesTemplateEventually(timeStep, maxLength, vars);
            }    
            case SkeletonType::always: {
                return genDualClausesTemplateAlways(timeStep, maxLength, vars);
            }    
            case SkeletonType::land: {
                return genDualClausesTemplateAnd(timeStep, maxLength, vars);
            }    
            case SkeletonType::lor: {
                return genDualClausesTemplateOr(timeStep, maxLength, vars);
            }    
            case SkeletonType::release: {
                return genDualClausesTemplateRelease(timeStep, maxLength, vars);
            }    
            case SkeletonType::until: {
                return genDualClausesTemplateUntil(timeStep, maxLength, vars);
            }    
            default: {
                assert(false);
            }

        }
        assert(false);
   }

