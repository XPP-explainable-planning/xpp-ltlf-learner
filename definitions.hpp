#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>
#include <assert.h>
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
