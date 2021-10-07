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
        Analyzes the vocabularies for static facts which will just increase the sat problem size.
        @param voc  the current vocabulary
        @param posExamples the positive examples to analyze
        @param negExamples the negative examples to analyze

        @return A vocabulary without facts occuring in every trace in every position
    */
    vector<string> analyseVoc(vector<string> voc, vector<vector<vector<string>>> posExamples, vector<vector<vector<string>>> negExamples){
        vector<string> newVoc;
        for (string fact : voc) {
            bool exists = true;
            for (vector<vector<string>> plan : posExamples) {
                if (!exists)
                    break;
                for (vector<string> state : plan) {
                    if (!exists)
                        break;
                    if (find(state.begin(), state.end(),fact) == state.end())
                        exists = false;
                    }
            }

            for (vector<vector<string>> plan : negExamples) {
                if (!exists)
                    break;
                for (vector<string> state : plan) {
                    if (!exists)
                        break;
                    if (find(state.begin(), state.end(),fact) == state.end())
                        exists = false;
                    }
            }
            if (!exists) {
                newVoc.push_back(fact);
            } 
        }
        return newVoc;
    }

    /**
        Analyzes the vocabularies for static facts which will just increase the sat problem size.
        @param voc  the current vocabulary
        @param examples the examples to analyze

        @return A vocabulary without facts occuring in every trace in every position
    */
    vector<string> analyseVocSingle(vector<string> voc, vector<vector<vector<string>>> examples){
        vector<string> newVoc;
        for (string fact : voc) {
            bool exists = true;
            for (vector<vector<string>> plan : examples) {
                if (!exists)
                    break;
                for (vector<string> state : plan) {
                    if (!exists)
                        break;
                    if (find(state.begin(), state.end(),fact) == state.end())
                        exists = false;
                }
            }
            if (!exists) {
                newVoc.push_back(fact);
            }
        }
        return newVoc;
    }
    

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
        cout << "--examples-per-permutation [number] to set the subsets size for learning " << endl;
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
    This method tries to learn n formulas which hold for the positive examples and do not hold for the bad examples.
    @param positiveExampleFolder the directory where the parser finds the positive examples 
    @param negativeExamplesFolder the directory where the parser finds the negative examples 
    @param plansPerPermuation the number of examples taken in to account per permuation
    @param numPermutations the number of permutations
    @param formulaCount the maximum number of formulas to be learned
    @param formulaSize the maximum size of the learned formula

    @return 0 if success, error otherwise.
*/
int learnContrastiveFormulas(char* positiveExamplesFolder, char* negativeExamplesFolder, 
                            int plansPerPermutation, int numPermutations,
                            int formulaCount, int formulaSize,
                            vector<Operator*> metaOperators,
                            bool stateBased, vector<string> ignorePredicates
                            ) {
        std::map< std::string, std::set<int> > mymap;
        Parser* p = new Parser(stateBased, ignorePredicates);
        vector<vector<vector<string > > > goodExamples = readExamplesFromFolder(positiveExamplesFolder, p);
        vector<vector<vector<string > > > badExamples = readExamplesFromFolder(negativeExamplesFolder, p);
        vector<string> voc = analyseVoc(p->getVocabulary(), goodExamples, badExamples);

        
        if (goodExamples.size() == 0) {
            cout << "No positive Examples found" << endl;
            return 0;
            }
        if (badExamples.size() == 0) {
            cout << "No negative Examples found" << endl;
            return 0;
            }

        //Either set the permutation Size to maximum number of plans or set number. 
        int permutationsGood = plansPerPermutation;
        int permutationsBad= plansPerPermutation;
        if (plansPerPermutation == -1 || goodExamples.size() < plansPerPermutation || badExamples.size() < plansPerPermutation) {
            permutationsGood = goodExamples.size();
            permutationsBad = badExamples.size();
        }
        //Run the learner for each permutation
        int count = 0;
        int processed = 0;
        std::random_device rd;
        std::mt19937 g(rd());
        vector<int> pos_examples;
        vector<int> neg_examples;
        for (int i = 0; i < goodExamples.size(); i++){
            pos_examples.push_back(i);
        }
        for (int i = 0; i < badExamples.size(); i++){
            neg_examples.push_back(i);
        }
        for (int processed = 0; processed < numPermutations; processed++) {
                std::shuffle(pos_examples.begin(), pos_examples.end(), g);
                std::shuffle(neg_examples.begin(), neg_examples.end(), g);
                bool learned = false;
                int currentSize = 1;
                //cout << "Learning..." << endl;
                vector<string> learnedFormulas;
                if (!metaOperators.empty()){
                    formulaSize = 3;
                }
                // Learn until [formulaCount] many formulas are learned
                while (!learned && currentSize++<formulaSize){
                    count++;
                    Cnf* cnf;
                    //Use this for filtering out arbitrary solutions while using meta-operators
                    /*if (currentSize == 2 && !metaOperators.empty()) {
                        cnf = new Cnf(currentSize, voc, createFinallyGlobally(), false);
                    } else { */
                        cnf = new Cnf(currentSize, voc, metaOperators, false);
                   // }

                    for (int i = 0; i < permutationsGood; i++) {
                        cnf->addPositiveExample(goodExamples.at(pos_examples.at(i)));
                    }
                    for (int i = 0; i < permutationsBad; i++) {
                        cnf->addNegativeExample(badExamples.at(neg_examples.at(i)));
                    }
                    learnedFormulas = cnf->learnFormulas(formulaCount);
                    delete(cnf);
                    if (learnedFormulas.size())
                        learned = true;
                }
                for (string s : learnedFormulas) {
                    map<string, set<int>>::iterator found = mymap.find(s);
                    if (found == mymap.end()){
                        set<int> temp;
                        for (int p = 0; p < permutationsGood; p++) {
                            temp.insert(pos_examples.at(p));
                        }
                        mymap.insert(make_pair(s,temp));
                    } else {
                        for (int p = 0; p < permutationsGood; p++) {
                            found->second.insert(pos_examples.at(p));
                        }
                    }
                }
                processed++;
                if (processed >= numPermutations)
                    break;
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
/**
This method tries to learn from a unique set of plans.
 First it selects random plans an marks them as good and bad plans and then invokes the learner.
 Upon learning a formula differentiating those sets of plans, an automaton is built representing the 
 learned Formula and then a run for all plans is performed.

    @param mixedExamplesFolder the directory where the parser finds the positive examples 
    @param plansPerPermuation the number of examples taken in to account per permuation
    @param numPermutations the number of permutations
    @param formulaCount the maximum number of formulas to be learned
    @param formulaSize the maximum size of the learned formula

    @return 0 if success, error otherwise.
    */
int learnMixedFormulas(char* mixedExamplesFolder, int plansPerPermutation, int numPermutations, 
                        int formulaCount, int formulaSize, vector<Operator*> metaOperators,
                        int stateBased, vector<string> ignorePredicates
                        ){
        std::map< std::string, int> mymap;
        std::map< std::string, int> formulaSizeMap;
        Parser* p = new Parser(stateBased, ignorePredicates);
        vector<vector<vector<string > > > examples = readExamplesFromFolder(mixedExamplesFolder, p);
        vector<vector<vector<string> > > mappedExamples;
        vector<string> voc =analyseVocSingle(p->getVocabulary(), examples);

        //Create mapping for all vocabulary to remove ',' for example
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
        /**
        for creating the permutations
        */
        vector<int> exampleInts;
        for (int i = 0; i < examples.size(); i++) {
            exampleInts.push_back(i);
        }
        int permutationsGood = plansPerPermutation;
        if (examples.size() == 0) {
            cout << "No positive Examples found" << endl;
            return 0;
        }
        assert(permutationsGood * 2 < examples.size());
        int count = 0;
        for (int pCount = 0; pCount < numPermutations; pCount++) {
            bool learned = false;
            int currentSize = 1;
            if (! metaOperators.empty()){
                formulaSize = 3;
            } 
            std::random_device rd;
            std::mt19937 g(5);
            std::shuffle(exampleInts.begin(), exampleInts.end(), g);
           while (!learned && currentSize++ <formulaSize){
                vector<string> learnedFormulas;
                    count++;
                    Cnf* cnf = new Cnf(currentSize, remappedVoc,metaOperators, true);
            
                    for (int i = 0; i < plansPerPermutation; i++) {
                        cnf->addPositiveExample(mappedExamples.at(exampleInts.at(i)));
                    }
                    for (int i = 0; i < plansPerPermutation; i++) {
                        cnf->addNegativeExample(mappedExamples.at(exampleInts.at(i + plansPerPermutation)));
                    }
                    learnedFormulas = cnf->learnFormulas(formulaCount);
                    delete(cnf);
                    if (learnedFormulas.size() > 0)
                        learned = true;
                    ofstream outfile;
                    outfile.open("formulasBackup.txt", fstream::out | fstream::app);
                    // check learned formulas for run.
                    for (string formula : learnedFormulas) {
                        Automaton* at = generateAutomaton(formula);
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
                        formulaSizeMap.insert(make_pair(formula,currentSize));
                    }
                    outfile.close();
                }
            }
        ofstream outfile;
        outfile.open("formulas.txt", fstream::out | fstream::app);
        cout << "################ Learned Formulas  ################"<< endl;
        for (auto it = mymap.cbegin(); it != mymap.cend(); ++it){
            cout << left <<setw(30) << it->first << " : "<< it->second << endl;
            outfile << left << setw(30) << it->first << " : " << it->second << " : " << formulaSizeMap.at(it->first)<< endl; 
        }
        cout << "###################################################"<< endl;
        outfile.close();
        return 0;
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
      //  return 1;
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
   //}
    cout << endl;*/
    char* mixedExamplesFolder = 0;
    char* positiveExamplesFolder = 0;
    char* negativeExamplesFolder = 0;
    char* metaOperatorFile = 0;
    bool generate = false;
    int formulaSize = 2;
    bool stateBased = false;
    bool randomExamples = false;
    bool statistics = false;
    int formulaCount = -1;
    int plansPerPermutation = -1;
    int numPermutations = 1000;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i],"-g") == 0) {
            generate = true;
        }
        if (strcmp(argv[i],"-f") == 0) {
             mixedExamplesFolder = argv[(i++)+1];
        }
        if (strcmp(argv[i],"-p") == 0) {
             positiveExamplesFolder = argv[(i++)+1];
        }
        if (strcmp(argv[i],"-n") == 0) {
             negativeExamplesFolder = argv[(i++)+1];
        }
        if (strcmp(argv[i], "-s") == 0) {
             char* size = argv[(i++)+1];
             formulaSize = atoi(size);
        }
        if (strcmp(argv[i] , "-a") == 0) {
            stateBased = true;
        }
        if (strcmp(argv[i] , "-r") == 0) {
            randomExamples = true;
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
        if (strcmp(argv[i] , "--examples-per-permutation") == 0) {
            char* count = argv[(i++)+1];
            plansPerPermutation = atoi(count);
        }
        if (strcmp(argv[i] , "--permutation-count") == 0) {
            char* count = argv[(i++)+1];
            numPermutations = atoi(count);
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

    if (positiveExamplesFolder != 0 && randomExamples) {
        return learnContrastiveFormulas(positiveExamplesFolder, negativeExamplesFolder, 
        plansPerPermutation, numPermutations, formulaCount, formulaSize, metaOperators, stateBased, ignorePredicates);
    }
    
    if (mixedExamplesFolder != 0 && randomExamples) {
        return learnMixedFormulas(mixedExamplesFolder, plansPerPermutation, numPermutations, formulaCount, formulaSize, metaOperators, stateBased, ignorePredicates);
    }

    /*
        The case where good and bad examples are located in one folder
        Negative Examples starting with prefix 'bad'
        Postitive Examples start with 'good'
    */
    if (mixedExamplesFolder != 0 && !randomExamples) {
        Parser* p = new Parser(stateBased, ignorePredicates);
        vector<vector<vector<string > > > badExamples = readBadExamplesFromFolder(mixedExamplesFolder, p);
        vector<vector<vector<string > > > goodExamples = readPosExamplesFromFolder(mixedExamplesFolder, p);
        vector<string> voc = p->getVocabulary();
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
