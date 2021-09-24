#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "definitions.hpp"


/**
 *  The main data structure for creating and recovering the learned formula from the SAT encoding
 *  This is based on the implementation of the Paper "Learning Interpretable Models Expressed in Linear Temporal Logic"- Camacho,McIlraith
 *  
 *  Depending on the usecase of the variable, different values are set
 *
 *  varType can be either 
 *      ETS mapping to an plan(exampleid, timestep, skeletonid)
 *      ST(SkeletonType), defining a node in the to be learned formula
 *      Alpha/Beta, defining the connections in node tree of the formula
 *      skliteral, defining a leaf (forcing it to be a fact)
 *
 *
 *      id = the id of this variable to recover the data and set them in the SAT-sovler
 *      exampleId is set if this variable is needed for an explicit plan
 *      timestep - mapping to an explicit state in the plan
 *      skeletonId - mapping to a Node in the formulaStructure
 *      skeletonType - mapping to a LTL Operator for the skeletonId node 
 *      alpha - mapping to the first subformula skeleton
 *      beta - mapping to the second subformula skeleton (for binary operators)
 *      literal - mapping to a fact/action
 *
 */
class Variable {
    public:
        Variable(VarType type):varType(type){};
        VarType varType;
        int id;
        int exampleId;
        int timestep;
        int skeletonId;
        int skeletonType;
        int alpha;
        int beta;
        int literal;
};


#endif
