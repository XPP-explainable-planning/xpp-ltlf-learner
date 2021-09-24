#include "operatorParser.hpp"
#include <fstream>
#include "operatorTemplate.hpp"
#include <sstream>
#include <iostream>
#include "metaOperator.cpp"
#include <assert.h>

/**
 *  Main parsing function, parsing a current line forward as Operator template,
 *
 *  For example a line looks like this: R R F a a b 
 *  
 *  @param operators - the metaOperator string from file
 *  @return A operator template implementing the treestructure of the metaoperator
 */ 
OperatorTemplate* OParser::parseInTemplate(string& operators){
    while (operators[0] == ' ') {
        operators = operators.substr(1);
    }
    if (operators[0] == '\n'){
        assert(false);
    }
    
    if (operators [0] == '!'){
        operators = operators.substr(1);
        OperatorTemplate* tmpl = parseInTemplate(operators);
        assert(tmpl->leaf != 0);
        tmpl->leaf = tmpl->leaf + 1; // alpha is 1 and gets 2, beta is 3 and becomes 4
        return tmpl;
    }
    if (operators[0] == 'a'){
        return new OperatorTemplate(SkeletonType::literal, false, 1);
    }
    if (operators[0] == 'b'){
        return new OperatorTemplate(SkeletonType::literal, false, 3);
    }
    
    if (operators[0] == 'G') {
        OperatorTemplate* tmpl = new OperatorTemplate(SkeletonType::always, true, 0);
        operators = operators.substr(1);
        tmpl->varId = this->currentVarId++;
        cout << tmpl->varId << endl;
        tmpl->rhs = parseInTemplate(operators);
        return tmpl;
    }
    if (operators[0] == 'F') {
        OperatorTemplate* tmpl = new OperatorTemplate(SkeletonType::eventually, true, 0);
        operators = operators.substr(1);
        tmpl->varId = this->currentVarId++;
        cout << tmpl->varId << endl;
        tmpl->rhs = parseInTemplate(operators);
        return tmpl;
    }
    if (operators[0] == 'X') {
        OperatorTemplate* tmpl = new OperatorTemplate(SkeletonType::next, true, 0);
        operators = operators.substr(1);
        tmpl->varId = this->currentVarId++;
        cout << tmpl->varId << endl;
        tmpl->rhs = parseInTemplate(operators);
        return tmpl;
    }
    if (operators[0] == '|') {
        OperatorTemplate* tmpl = new OperatorTemplate(SkeletonType::lor, false, 0);
        operators = operators.substr(1);
        tmpl->varId = this->currentVarId++;
        cout << tmpl->varId << endl;
        tmpl->lhs = parseInTemplate(operators);
        operators = operators.substr(1);
        tmpl->rhs = parseInTemplate(operators);
        return tmpl;
    }
    if (operators[0] == '&') {
        OperatorTemplate* tmpl = new OperatorTemplate(SkeletonType::land, false, 0);
        operators = operators.substr(1);
        tmpl->varId = this->currentVarId++;
        cout << tmpl->varId << endl;
        tmpl->lhs = parseInTemplate(operators);
        operators = operators.substr(1);
        tmpl->rhs = parseInTemplate(operators);
        return tmpl;
    }
    if (operators[0] == 'U') {
        OperatorTemplate* tmpl = new OperatorTemplate(SkeletonType::until, false, 0);
        operators = operators.substr(1);
        tmpl->varId = this->currentVarId++;
        cout << tmpl->varId << endl;
        tmpl->lhs = parseInTemplate(operators);
        operators = operators.substr(1);
        tmpl->rhs = parseInTemplate(operators);
        return tmpl;
    }
    if (operators[0] == 'R') {
        OperatorTemplate* tmpl = new OperatorTemplate(SkeletonType::release, false, 0);
        operators = operators.substr(1);
        tmpl->varId = this->currentVarId++;
        cout << tmpl->varId << endl;
        tmpl->lhs = parseInTemplate(operators);
        operators = operators.substr(1);
        tmpl->rhs = parseInTemplate(operators);
        return tmpl;
    }
    assert(false);
}

/**
 *  Checks the leafs if they have one or two different literals
 *
 *  @param metaTemplate - the metaOperator
 *
 *  @return true if it is binary, false otherwise
 *  
 *
 */ 
bool checkIfBinary(OperatorTemplate* metaTemplate) {
    if (metaTemplate->leaf != 0)
        return metaTemplate->leaf >2;
    if (metaTemplate->isUnary)
        return checkIfBinary(metaTemplate->rhs);
    return checkIfBinary(metaTemplate->rhs) || checkIfBinary(metaTemplate->lhs);
}

/**
 *  
 *  The parsefunction calling the parser and assigning the metaOperator to its operatorType(skeletonType)
 *  @param currentOperator - the metaOperator as string
 *  @param type - the typeId of the operator
 *
 *  @returns a built metaoperator
 *
 */
Operator* OParser::parseMetaOperator(string currentOperator, int type){
    OperatorTemplate* metaTemplate = parseInTemplate(currentOperator);
    bool isBinary = checkIfBinary(metaTemplate);
    //cout << "Parsed MetaOperator: "  << metaTemplate->printMe("lhs", "rhs", true) << "  which is " << isBinary << "as type " << type << endl;
    
    return (Operator*) new MetaOperator(type,isBinary, metaTemplate, this->currentVarId);
}



/**
 *  The fileparser for metaOperators, parsing one metaOperator per line.
 *
 *  @param filename - the filename, containing the metaOperators.
 *
 * An example File :
 *
 * R F a b a
 * F G a 
 * G || a b
 * && G a G b
 *  
 *  @return a list containing all the metaOperators.
 *
 */
vector<Operator*> OParser::parseMetaOperators(char* filename) {
    
    int operatorType = 9;

    vector<Operator*> metaOperators;
    ifstream operatorFile(filename);
    this->currentVarId = 5;
    if (operatorFile.is_open()) {
        string currentLine;
        while(true) {
            if (operatorFile.eof())
                break;
            
            getline(operatorFile,currentLine);
            if (currentLine.size() == 0) {
                break;
            }
            this->currentVarId = 5;
            metaOperators.push_back(parseMetaOperator(currentLine,operatorType++));
        }

    }
    operatorFile.close();
    return metaOperators;
}
