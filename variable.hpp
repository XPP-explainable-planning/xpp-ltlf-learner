#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "definitions.hpp"

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
