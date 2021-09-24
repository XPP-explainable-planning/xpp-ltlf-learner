#ifndef OPERATOR_PARSER_HPP
#define OPERATOR_PARSER_HPP

#include "../../definitions.hpp"
#include "../operator.hpp"
#include "operatorTemplate.hpp"


using namespace std;

/**
 *  An operator parser parsing and creating the operator templates.
 *
 */
class OParser {
private:
int currentVarId;
OperatorTemplate* parseInTemplate(string& operators);
Operator* parseMetaOperator(string currentOperator, int type);
public:
vector<Operator*> parseMetaOperators(char* filename);


};

#endif
