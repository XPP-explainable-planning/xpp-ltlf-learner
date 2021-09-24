#include <fstream>
#include <iostream>
#include "cnf.hpp"
#include <sys/resource.h>
#include <vector>
#include <map>
#include <set>
#include <string.h>
#include "search.h"
#include "parser.hpp"
#include <filesystem>
#include <algorithm>
#include <random>
#include "definitions.hpp"
#include "traceChecking/automaton.hpp"
#include "operators/omegaOperators/operatorParser.hpp"

namespace fs = std::filesystem;




    /**
     *  Help function
     *
     */ 
    void printHelp() {
        cout << "-f [folder]" << endl;
        cout << "if -f is used, positive examples contain 'good', negative examples should contain 'bad'" << endl;
        cout << "-p [folder]" << endl;
        cout << "-n [folder]" << endl; 
        cout << "-c [formulaSize]" << endl;
        cout << "-g to generate Traces" << endl;
        cout << "-a to parse state based traces" << endl;
        cout << "-r to just get a simple input set" << endl;
        cout << "-s to size " << endl;
        cout << "-v to verbose output" << endl;
        cout << "--permutation-count number of permutations which will be runned" << endl;
        cout << "--formula-count amout of formulas" << endl;
        cout << "-t testOnly" << endl;
        cout << "--permutations [number] to set the subsets size for learning " << endl;
        cout << "--meta-operators <file> to define own operators" << endl;
    }

    /**
     * Reads all files from this folder and parses them into examplePlans
     *
     * @param folderName - the path to the folder
     * @param parser - the instantiated parser
     * @return a list of the parsed plans
     */
    vector<vector<vector<string> > > readExamplesFromFolder(char* folderName, Parser *parser) {
        vector<vector<vector<string> > > parsedTraces;
        for (const auto &entry : fs::directory_iterator(folderName)) {
            char* fPath =(char*) entry.path().c_str();
            vector<vector<string> > trace = parser->parse(fPath);
            parsedTraces.push_back(trace);
        }
        return parsedTraces;
    }


    /**
     * Reads all files from this folder and parses them into examplePlans,
     * positive plans usually start in this case with the prefix good.
     *
     * @param folderName - the path to the folder
     * @param parser - the instantiated parser
     * @return a list of the parsed plans
     */
    vector<vector<vector<string> > > readPosExamplesFromFolder(char* folderName, Parser *parser) {
        vector<vector<vector<string> > > parsedTraces;
        for (const auto &entry : fs::directory_iterator(folderName)) {
            if (((string)entry.path().filename()).find("good") == 0) { 
                char* fPath =(char*) entry.path().c_str();
                vector<vector<string> > trace = parser->parse(fPath);
                parsedTraces.push_back(trace);
            }
        }
        return parsedTraces;

    }

    /**
     * Reads all files from this folder and parses them into examplePlans,
     * positive plans usually start in this case with the prefix "bad".
     *
     * @param folderName - the path to the folder
     * @param parser - the instantiated parser
     * @return a list of the parsed plans
     */
    vector<vector<vector<string> > > readBadExamplesFromFolder(char* folderName, Parser *parser) {
        vector<vector<vector<string> > > parsedTraces;
        for (const auto &entry : fs::directory_iterator(folderName)) {
            if (((string)entry.path().filename()).find("bad") == 0) { 
                char* fPath =(char*) entry.path().c_str();
                vector<vector<string> > trace = parser->parse(fPath);
                parsedTraces.push_back(trace);
            }
        }
        return parsedTraces;

    }

/**
 *      Support function for replacing characters in a string, used for removing "(" for the automaton generation
 *      
 *      @param data - string containg the data
 *      @param match - string which has to be replaced
 *      @param replace - the string which replaces the match
 
 *
 */
void findAndReplaceAll( std::string& data,  
                        const std::string& match,  
                        const std::string& replace) 
{ 
   // Get the first occurrence 
   size_t pos = data.find(match); 
 
   // Repeat till end is reached 
   while( pos != std::string::npos) 
    { 
        data.replace(pos, match.size(), replace); 
      
       // Get the next occurrence from the current position 
        pos = data.find(match, pos+replace.size()); 
    } 
} 
 

/**
 *  
 *  Main function,
 *  Parsing and then invoking the learner according to the input,
 *  
 *  1. Learning Contrastive Formulas 
 *   ../../learner/build/learner -p positivePlans/stateBased -n negativePlans/stateBased --formula-count -1 -a -s 4 -r --permutations 20 --permutation-count 1
 *   -p positive plans
 *   -n negative plans
 *   --formula-count maximum numbers of formulas
 *   -a  parserargument for using right parser
 *   -s 4 formula size bound
 *   --permutations = number of positive/negative plans per call
 *   --permutation-count = number of instantiations
 * 2. Learning Blind 
 *   ../../learner/build/learner -f stateBased --formula-count 5 -a -s 4 -r --permutations 3 --permutation-count 10 
 *  for using metaoperators, add the flag
 *  --meta-operators ../../learner/operators.txt
 */

int main(int argc, char* argv[]) {

    long max_limit = 2 * 1024 * 1024 * 1024l;  // 2 GB
    struct rlimit limit;
    limit.rlim_cur = RLIM_INFINITY;
    limit.rlim_max = max_limit;
    if (0 != setrlimit(RLIMIT_AS, &limit)) {
        return 1;
    }


    vector<string> ignorePredicates;
    /*
    ignorePredicates.push_back("connected");
    ignorePredicates.push_back("fuel");
    ignorePredicates.push_back("can_traverse");
    ignorePredicates.push_back("visible");
    ignorePredicates.push_back("ecost");
    ignorePredicates.push_back("energy");
     */
   // cout << "Ignoring Predicates: ";
   /* for (string s : ignorePredicates) {
        cout << s << "  " ;
    }
    cout << endl;*/
    char* fFolder = 0;
    char* pFolder = 0;
    char* nFolder = 0;
    char* metaOperatorFile = 0;
    bool generate = false;
    int formulaSize = 2;
    bool stateBased = false;
    bool random = false;
    bool statistics = false;
    int formulaCount = -1;
    int max_num_pos_examples = -1;
    int max_num_neg_examples = -1;
    bool testOnly = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i],"-g") == 0) {
            generate = true;
        }
        if (strcmp(argv[i],"-t") == 0) {
            testOnly = true;
        }
        if (strcmp(argv[i],"-f") == 0) {
             fFolder = argv[(i++)+1];
        }
        if (strcmp(argv[i],"-p") == 0) {
             pFolder = argv[(i++)+1];
        }
        if (strcmp(argv[i],"-n") == 0) {
             nFolder = argv[(i++)+1];
        }
        if (strcmp(argv[i], "-s") == 0) {
             char* size = argv[(i++)+1];
             formulaSize = atoi(size);
        }
        if (strcmp(argv[i] , "-a") == 0) {
            stateBased = true;
        }
        if (strcmp(argv[i] , "-r") == 0) {
            random = true;
        }
        if (strcmp(argv[i] , "-v") == 0) {
            statistics = true;
        }
        if (strcmp(argv[i] , "-h") == 0) {
            printHelp();
            return 0;
        }
        if (strcmp(argv[i] , "--formula-count") == 0) {
            char* count = argv[(i++)+1];
            formulaCount = atoi(count);
        }
        if (strcmp(argv[i] , "--max-num-pos-examples") == 0) {
            char* count = argv[(i++)+1];
            max_num_pos_examples = atoi(count);
        }
        if (strcmp(argv[i] , "--max-num-neg-examples") == 0) {
            char* count = argv[(i++)+1];
            max_num_neg_examples = atoi(count);
        }
        if (strcmp(argv[i] , "--meta-operators") == 0) {
            metaOperatorFile = argv[(i++)+1];
        }
    }
    vector<Operator*> metaOperators;
    if (metaOperatorFile != 0){
        OParser oParser;
        metaOperators = oParser.parseMetaOperators(metaOperatorFile);
    }

    if (pFolder != 0 && random) {
        std::map< std::string, std::set<int> > mymap;
        Parser* p = new Parser(stateBased, ignorePredicates);
        vector<vector<vector<string > > > goodExamples = readExamplesFromFolder(pFolder, p);
        vector<vector<vector<string > > > badExamples = readExamplesFromFolder(nFolder, p);
        vector<string> voc = p->getVocabulary();

        if (goodExamples.size() >= max_num_pos_examples) {
            cout << "No enough positive Examples found" << endl;
            return 1;
            }
        if (badExamples.size() >= max_num_neg_examples) {
            cout << "No enough negative Examples found" << endl;
            return 1;
            }

        int count = 0;
        int processed = 0;
        std::random_device rd;
        std::mt19937 g(5);
        vector<int> pos_examples;
        vector<int> neg_examples;
        for (int i = 0; i < goodExamples.size(); i++){
            pos_examples.push_back(i);
        }
        for (int i = 0; i < badExamples.size(); i++){
            neg_examples.push_back(i);
        }


        bool learned = false;
        int currentSize = 1;
        //cout << "Learning..." << endl;
        vector<string> learnedFormulas;
        if (!metaOperators.empty()){
            currentSize++;
            formulaSize = 3;
        }
        while (!learned && currentSize++<formulaSize){
            count++;
           /* cout << count << ": (";
            for (int p : posComb)
                cout << p << ", ";
            cout << ")  &  (";
            for (int p : badComb)
                cout << p << ", ";
            cout << ")" << endl;
            */
            cout << "starting with size " << currentSize << endl;
            Cnf* cnf = new Cnf(currentSize, voc, metaOperators, false);
            for (int i = 0; i < max_num_pos_examples; i++) {
                cnf->addPositiveExample(goodExamples.at(pos_examples.at(i)));
            }
            for (int i = 0; i < max_num_neg_examples; i++) {
                cnf->addNegativeExample(badExamples.at(neg_examples.at(i)));
            }
            learnedFormulas = cnf->learnFormulas(formulaCount);
            delete(cnf);
            if (learnedFormulas.size())
                learned = true;
        }
        for (string s : learnedFormulas) {
           // cout << s << endl;
            map<string, set<int>>::iterator found = mymap.find(s);
            if (found == mymap.end()){
                set<int> temp;
                for (int p = 0; p < max_num_pos_examples; p++) {
                    temp.insert(pos_examples.at(p));
                }
                mymap.insert(make_pair(s,temp));
            } else {
                for (int p = 0; p < max_num_neg_examples; p++) {
                    found->second.insert(pos_examples.at(p));
                }
            }
        }

        ofstream outfile;
        outfile.open("formulas.txt", fstream::out);
        cout << "################ Learned Formulas  ################"<< endl;
        for (auto it = mymap.cbegin(); it != mymap.cend(); ++it){
            cout << left <<setw(30) << it->first << endl; //" : "<< float(it->second.size())/float(numPermutations) << endl;
            outfile << it->first << endl;
        }
        outfile.close();
        cout << "###################################################"<< endl;

        return 0;

    }
    
    if (fFolder != 0 && random) {
        std::map< std::string, int> mymap;
        Parser* p = new Parser(stateBased, ignorePredicates);
        vector<vector<vector<string > > > examples = readExamplesFromFolder(fFolder, p);
        vector<vector<vector<string> > > mappedExamples;
        vector<string> voc = p->getVocabulary();
        vector<string> remappedVoc;
        for (int i = 0; i < voc.size(); i ++){
            remappedVoc.push_back("a" + to_string(i) + "a");
        }
        for (vector< vector< string > > example : examples){
            vector<vector<string > > remapped;
            for (vector<string> state : example){
                vector<string> remappedState;
                for (string fact : state) {
                    for (int pos = 0; pos < voc.size(); pos++) {
                        if (fact.compare(voc.at(pos)) == 0){
                            remappedState.push_back("a" + to_string(pos) + "a");
                            break;
                        }
                    }
                }
                remapped.push_back(remappedState);
            }
            mappedExamples.push_back(remapped);
        }
        vector<int> exampleInts;
        for (int i = 0; i < examples.size(); i++) {
            exampleInts.push_back(i);
        }
        int permutationsGood = max_num_pos_examples;
        if (examples.size() >= max_num_pos_examples) {
            cout << "No positive Examples found" << endl;
            return 0;
            }
        cout << max_num_pos_examples << endl;
        assert(permutationsGood * 2 < examples.size());
        int count = 0;
        for (int pCount = 0; pCount < max_num_pos_examples; pCount++) {
            bool learned = false;
            int currentSize = 1;
            if (! metaOperators.empty()){
                currentSize = 2;
                formulaSize = 3;
            }
            //shuffle the examples to get different results
           while (!learned && currentSize++ <formulaSize){
                std::random_device rd;
                std::mt19937 g(rd());
                cout << currentSize << endl;
                std::shuffle(exampleInts.begin(), exampleInts.end(), g);
                //cout << "Learning..." << endl;
                vector<string> learnedFormulas;
                    count++;
                    Cnf* cnf = new Cnf(currentSize, remappedVoc,metaOperators, true);
                    for (int i = 0; i < max_num_pos_examples; i++) {
                        cnf->addPositiveExample(mappedExamples.at(exampleInts.at(i)));
                    }
                    for (int i = 0; i < max_num_neg_examples; i++) {
                        cnf->addNegativeExample(mappedExamples.at(exampleInts.at(i + max_num_pos_examples)));
                    }
                    learnedFormulas = cnf->learnFormulas(25);
                    delete(cnf);
                    if (learnedFormulas.size() > 0)
                        learned = true;
                    ofstream outfile;
                    outfile.open("formulas.txt", fstream::out | fstream::app);
                    for (string formula : learnedFormulas) {
                        Automaton* at = generateAutomaton(formula);
                        //at->printAutomaton();
                        int myCount = 0;

                        // Remap back to aps 
                        for (string ap : at->factsMap) {
                            int number = atoi(ap.substr(1,ap.size()-1).c_str());
                            findAndReplaceAll(formula, ap ,voc.at(number));
                        }
                        for (vector<vector<string > > plan : mappedExamples){
                            if (checkForTrace(at, plan))
                                myCount++;
                        }
                        outfile << left << setw(30) << formula << " : " << myCount << endl;
                        mymap.insert(make_pair(formula,myCount));
                    }
                    outfile.close();
                }
            }
        cout << "################ Learned Formulas  ################"<< endl;
        for (auto it = mymap.cbegin(); it != mymap.cend(); ++it){
            cout << left <<setw(30) << it->first << " : "<< it->second << endl;
        }
        cout << "###################################################"<< endl;

        return 0;
    }
    if (fFolder != 0 && !random) {
        cout << "parsing files" << endl;
        Parser* p = new Parser(stateBased, ignorePredicates);
        cout << "parsing negative examples" << endl;
        vector<vector<vector<string > > > badExamples = readBadExamplesFromFolder(fFolder, p);
        cout << "parsing positive examples" << endl;
        vector<vector<vector<string > > > goodExamples = readPosExamplesFromFolder(fFolder, p);
        vector<string> voc = p->getVocabulary();
        if (testOnly) {
            cout << "\\begin{tabular}{ccccccc}" << endl;
            cout << "#APs & #FormulaSize & #TraceLength & #Variables & #Clauses  & #VariablesPerTrace & #ClausesPerTrace\\\\" << endl;
            for (int i = 2; i < 13; i++) {
                Cnf *cnf = new Cnf(i, voc,metaOperators, false);
                for (vector<vector<string> > goodExample : goodExamples) {
                    cnf->addPositiveExample(goodExample);
                }
                for (vector<vector<string> > badExample : badExamples) {
                    cnf->addNegativeExample(badExample);
                }
                cnf->printTable();
                delete(cnf);
            }
            cout << "\\end{tabular}" << endl;
            return 0;
        }
        Cnf *cnf = new Cnf(formulaSize, voc,metaOperators,false);
        for (vector<vector<string> > goodExample : goodExamples) {
            cnf->addPositiveExample(goodExample);
        }
        for (vector<vector<string> > badExample : badExamples) {
            cnf->addNegativeExample(badExample);
        }
        cout << "learned Clauses:" << endl;
        for (string formula : cnf->learnFormulas(formulaCount)) {
            cout << formula << endl;
        }
        if (statistics)
            cnf->printStatistics();
    }
}
