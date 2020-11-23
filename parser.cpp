#include "parser.hpp"

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <fcntl.h>

#include <iostream>


    vector<string> Parser::getVocabulary() {
        vector<string> voc; 
        for (unordered_set<string>::iterator iter = vocabulary.begin(); iter != vocabulary.end(); iter++) {
            voc.push_back(*iter);
        }
        return voc;
    }

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}
    

 vector<vector<string> > Parser::parse(char* fn){
    if (stateBased)
       return parseStateTrace(fn);
    return parseTrace(fn);

 }


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


  bool Parser::checkIgnore(string current) {
        for (string ignores : ignorePredicates){
            if (current.find(ignores) !=string::npos){
                return true;
            }   
        }
        return false;
  }


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
