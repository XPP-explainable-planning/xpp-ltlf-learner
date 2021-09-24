#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>
#include <assert.h>

/**
 *
 * All possible skeletontypes, 
 * In theory all LTLf Operators and literal
 *
 */ 
enum SkeletonType {
    literal = 1,
    lor = 2,
    land = 3,
    next = 4,
    until = 5,
    release = 6,
    weaknext = 7,
    eventually = 8,
    always = 9,
};

/**
 *  Print function for Operators
 *  
 *  @param type - the LTL Operator as SkeletonType
 *  @return string representation of the Operator
 *
 */ 
static std::string printSkeletonType(SkeletonType type) {
    switch (type){
        case SkeletonType::lor : return "|| ";
        case SkeletonType::land : return "&& ";
        case SkeletonType::next : return "X ";
        case SkeletonType::until : return "U ";
        case SkeletonType::release : return "R ";
        case SkeletonType::weaknext : return "WX  ";
        case SkeletonType::eventually : return "F ";
        case SkeletonType::always : return "G ";
        case SkeletonType::literal: return "";
    }
}


/**
 *  Different possible Variable Types,
 *  differentiating on the usecase of the Variable
 *
 */ 
enum VarType{
    ETS = 1,
    ST = 2,
    alpha = 3,
    beta = 4,
    skliteral = 5,
    expredicate = 6,
    metaST = 7,
    metaETS = 8
};

static const VarType AllVarTypes[] = {ETS, ST, alpha, beta, skliteral, expredicate};

#endif
