#include "parser.hpp"

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <fcntl.h>

#include <iostream>


/**
 *  Returns the Vocabulary, the actions/facts occured in any parsed plan
 *
 *
 */

vector<string> Parser::getVocabulary() {
        vector<string> voc; 
        for (unordered_set<string>::iterator iter = vocabulary.begin(); iter != vocabulary.end(); iter++) {
            voc.push_back(*iter);
        }
        return voc;
}

/**
 *  Returns the filesize of the file,
 *  used for mmap later 
 *
 *  @param fn the filename
 *  @return size of the file
 */
size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}
    

/**
 *  main parsing function
 *
 *  The flag statebased is set in the constructor, to differentiate between actionbased or proposition based learning
 *
 *  @param fn the filename
 *  @return the vector of actions of the plan
 *  
 */
 vector<vector<string> > Parser::parse(char* fn){
    if (stateBased)
       return parseStateTrace(fn);
    return parseTrace(fn);

 }

/**
 *  File Parser for ActionBased Plan Files,
 *
 *  Example File:
 *
 *  (drop rover1 rover1store)
 *  (drive rover1 location1)
 *  ; cost 2 (unit cost)
 *
 *  Performs an mmap, maybe rewrite this if it causes problems
 *  Also adds new actions to the Vocabulary, which can be via the parser api
 *
 *  @param fn the filename
 *  @returns the vector of actions of the plan
 *
 *
 *  
 */
 vector<vector<string> > Parser::parseTrace(char* fn){
    size_t filesize = getFilesize(fn);
    int fd = open(fn, O_RDONLY, 0);

    assert (fd != -1);
    char* fp = (char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    
    vector<vector<string> > trace; 


    assert (fp != MAP_FAILED);
    while (true) {
        if (*fp == '(') {
            fp++;
            vector<string> currentPosition;
            string s = "";
            while (*fp != ')') {
                if (*fp == ' ') {
                    fp++;
                }
                else if (*fp == ',') {
                    assert(s.compare("") != 0);
                    currentPosition.push_back(s);
                    vocabulary.insert(s);
                    s = "";
                    fp++;
                }
                else {
                    s += *fp;
                    fp++;
                }
            }
            if (s.compare("") != 0) {
                currentPosition.push_back(s);
                vocabulary.insert(s);
            }
            trace.push_back(currentPosition);
            fp++;
            while(*fp != '\n'){
                fp++;
            }
        }
        if (*fp == ';') {
            return trace;
        }

        if (*fp == EOF) {
            assert(false);
        }
    fp++;
    }
}

  /**
   * Checks if the parsed fact to be ignored, sometimes fuel(truck0, level10) is not important
   * @param current the current parsed fact/action
   * @return true if the parser should ignore the fact/action, false otherwise
   */
  bool Parser::checkIgnore(string current) {
        for (string ignores : ignorePredicates){
            if (current.find(ignores) !=string::npos){
                return true;
            }   
        }
        return false;
  }

 /**
  *  The parser function for parsing fact-based files.
  *
  *  Usually a fact-based plan looks like this 
  *
  * [fact1 fact2 fact3]
  * [fact2 fact 4]
  * [fact5]
  * ;
  *
  * @param fn the filename
  * @return a vector containing the states of the plan
  *
  * Also adds new facts to the vocabulary
  *
  */
 vector<vector<string> > Parser::parseStateTrace(char* fn){
    size_t filesize = getFilesize(fn);
    int fd = open(fn, O_RDONLY, 0);
    assert (fd != -1);
    char* fp = (char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    
    vector<vector<string> > trace; 


    assert (fp != MAP_FAILED);
    while (true) {
        if (*fp == '[') {
            fp++;
            vector<string> currentPosition;
            string s = "";
            while (*fp != ']' ) {
                if (*fp == ' ') {
                    assert(s.compare("") != 0);
                    if (!checkIgnore(s)) {
                        currentPosition.push_back(s);
                        vocabulary.insert(s);
                    }
                    s = "";
                    fp++;
                }
                else {
                    s += *fp;
                    fp++;
                }
            }
            if (s.compare("") != 0) {
                if (!checkIgnore(s)) {
                    currentPosition.push_back(s);
                    vocabulary.insert(s);
                }
            }
            trace.push_back(currentPosition);
            fp++;
            while(*fp != '\n'){
                fp++;
            }
        }
        if (*fp == ';') {
            return trace;
        }
        if (*fp == EOF) {
            return trace;
        }
        fp++;
    }
}
