#include "operatorTemplate.hpp"


    /**
     *  Recursive function to generate additional variables for each node in the metaoperator
     *
     *  @param exampleId - the current example plan
     *  @param skeletonId - the current skeleton node
     *  @param maxLength - the length of the example plan
     *  @param dict - the dictionary for getting and pushing variables
     *  @param myId - the current id of the inner node starting at 10, then alpha subformula has id 100, beta has 101, and so on.
     *
     */
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

    /**
     *  Returns the id of the node, 
     *  leafs are directly mapped to literal skeletons
     *
     */
    int OperatorTemplate::getMetaId() { 
       return leaf ? leaf : varId;
    }
    
    /**
     *  
     *  Print function for printing the MetaOperator
     *  @param lhs - the string of the alpha literal
     *  @param rhs - the string of the beta literal
     *  @param normal - the print type - "R a b" or "a R b"
     *
     *  @returns the Metaoperator with literals as string.
     *
     */
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

