#include <fstream>
#include <iostream>
#include "cnf.hpp"
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

    vector<vector<vector<string> > > readExamplesFromFolder(char* folderName, Parser *parser) {
        vector<vector<vector<string> > > parsedTraces;
        for (const auto &entry : fs::directory_iterator(folderName)) {
            char* fPath =(char*) entry.path().c_str();
            vector<vector<string> > trace = parser->parse(fPath);
            parsedTraces.push_back(trace);
        }
        return parsedTraces;
    }


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
 



int main(int argc, char* argv[]) {


    vector<string> ignorePredicates;
    ignorePredicates.push_back("connected");
    ignorePredicates.push_back("fuel");
    ignorePredicates.push_back("can_traverse");
    ignorePredicates.push_back("visible");
    ignorePredicates.push_back("ecost");
    ignorePredicates.push_back("energy");
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
    int permutations = -1;
    bool testOnly = false;
    int numPermutations = 2000;
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
        if (strcmp(argv[i] , "--permutations") == 0) {
            char* count = argv[(i++)+1];
            permutations = atoi(count);
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

    if (pFolder != 0 && random) {
        std::map< std::string, std::set<int> > mymap;
        Parser* p = new Parser(stateBased, ignorePredicates);
        vector<vector<vector<string > > > goodExamples = readExamplesFromFolder(pFolder, p);
        vector<vector<vector<string > > > badExamples = readExamplesFromFolder(nFolder, p);
        vector<string> voc = p->getVocabulary();
        int permutationsGood = permutations;
        if (goodExamples.size() == 0) {
            cout << "No positive Examples found" << endl;
            return 0;
            }
        if (badExamples.size() == 0) {
            cout << "No negative Examples found" << endl;
            //return 0;
            }
        int permutationsBad= permutations;
        if (permutations == -1 || goodExamples.size() < permutations || badExamples.size() < permutations) {
            permutationsGood = goodExamples.size();
            permutationsBad = badExamples.size();
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
        for (int processed = 0; processed < numPermutations; processed++) {
                std::shuffle(pos_examples.begin(), pos_examples.end(), g);
                std::shuffle(neg_examples.begin(), neg_examples.end(), g);
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
                   // cout << s << endl;
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
        int permutationsGood = permutations;
        if (examples.size() == 0) {
            cout << "No positive Examples found" << endl;
            return 0;
            }
        cout << permutations << endl;
        assert(permutationsGood * 2 < examples.size());
        int count = 0;
        for (int pCount = 0; pCount < numPermutations; pCount++) {
            bool learned = false;
            int currentSize = 1;
            if (! metaOperators.empty()){
                currentSize = 2;
                formulaSize = 3;
            }
           while (!learned && currentSize++ <formulaSize){
                std::random_device rd;
                std::mt19937 g(5);
                cout << currentSize << endl;
                std::shuffle(exampleInts.begin(), exampleInts.end(), g);
                //cout << "Learning..." << endl;
                vector<string> learnedFormulas;
                    count++;
                    Cnf* cnf = new Cnf(currentSize, remappedVoc,metaOperators, true);
                    for (int i = 0; i < permutations; i++) {
                        cnf->addPositiveExample(mappedExamples.at(exampleInts.at(i)));
                    }
                    for (int i = 0; i < permutations; i++) {
                        cnf->addNegativeExample(mappedExamples.at(exampleInts.at(i + permutations)));
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
