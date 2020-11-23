#include "operatorTemplate.hpp"

    void OperatorTemplate::genVariablesForExamples(int exampleId, int skeletonId, int maxLength, VarsDict* dict, int myId){
        varId = myId;
        if (leaf != 0)
            return;
        for (int timeStep = 0; timeStep < maxLength; timeStep++){
           Variable* varETS = new Variable(VarType::metaETS);
           varETS->exampleId = exampleId;
           varETS->timestep = timeStep;
           varETS->skeletonId = skeletonId;
           varETS->alpha = varId;
           dict->addVariable(varETS);
        }
        int varRHS = varId * 10;
        rhs->genVariablesForExamples(exampleId, skeletonId, maxLength, dict, varRHS);
        if (!isUnary){
            int varLHS = varRHS + 1;
            lhs->genVariablesForExamples(exampleId, skeletonId, maxLength, dict, varLHS);
        }

    }


    int OperatorTemplate::getMetaId() { 
       return leaf ? leaf : varId;
    }
    
    string OperatorTemplate::printMe(string lhs, string rhs, bool normal){
        if (leaf == 1) {
            // Alpha 
            return lhs;
        }
        if (leaf == 2) {
            // ! Alpha
            return "! " + lhs;
        }

        if (leaf == 3) {
            // Beta
            return rhs;
        }
        if (leaf == 4) {
            // ! Beta
            return "!" + rhs;
        }
        
        if (isUnary){
            if (normal)
                return "( " + printSkeletonType(type) + " " + this->rhs->printMe(lhs, rhs, normal) + " )";
            return printSkeletonType(type) + " " + this->rhs->printMe(lhs, rhs, normal);
        }

        if (normal)
            return "( " +this->lhs->printMe(lhs,rhs,normal)  + " " + printSkeletonType(type) + " " + this->rhs->printMe(lhs,rhs,normal) + " )";
        return printSkeletonType(type) + this->lhs->printMe(lhs, rhs, normal)+ " " + this->rhs->printMe(lhs, rhs, normal); 
    }

