import spot
import buddy
spot.setup()
import random
import math
import os
import sys, getopt

class ExpandableSearch:
    def __init__(self, state, trace):
        self.state = state
        self.trace = trace



def create_Examples(formula, examplecount, mintracelength):
    automaton = formula.translate('ba', 'deterministic', 'complete')
    num_states = automaton.num_states()
    init = automaton.get_init_state()
    iterator = automaton.succ_iter(init)
    initialSearch = ExpandableSearch(init, [])
    toExpand = [initialSearch]
    nextLvl = []
    fullfills = []
    count = 0
    while count < examplecount:  
        for e in toExpand:
            iterator = automaton.succ_iter(e.state)
            if iterator.first():
                bdd = iterator.cond()
                trace = e.trace.copy()
                trace.append(spot.bdd_format_formula(automaton.get_dict(),bdd))
                state = iterator.dst()
                newElement = ExpandableSearch(state, trace)
                if len(trace) >= mintracelength:
                    if (automaton.state_is_accepting(state)):
                        fullfills.append(newElement)
                        count += 1
                nextLvl.append(newElement)
            while iterator.next():
                bdd = iterator.cond()
                trace = e.trace.copy()
                trace.append(spot.bdd_format_formula(automaton.get_dict(),bdd))
                state = iterator.dst()
                newElement = ExpandableSearch(state, trace)
                if len(trace) >= mintracelength:
                    if (automaton.state_is_accepting(state)):
                        fullfills.append(newElement)
                        count += 1
                nextLvl.append(newElement)
        toExpand = nextLvl.copy()
        nextLvl = []
    return fullfills

def chooseRandom(vocabulary):
    trace_element = []
    for v in vocabulary:
        rand = random.randint(0,1)
        if rand == 1:
            trace_element.append(v)
    return trace_element

def split_trace(trace):
    split = trace.split("|")
    if len(split) == 1:
        return split[0]
    rand = random.randint(0, len(split)-1)
    newTrace = split[rand]
    if newTrace[0] == ' ':
        newTrace = newTrace[1:]
    if newTrace[0] == '(':
        newTrace = newTrace[1:]
    if newTrace[len(newTrace)-1] == ' ':
        newTrace = newTrace[:-1]
    if newTrace[len(newTrace)-1] == ')':
        newTrace = newTrace[:-1]
    return newTrace


        

def trace_to_variable(trace, vocabulary):
    result = []
    for aps in trace:
        print(aps)
        aps = split_trace(aps)

        print(aps)
        formula = aps.split(" ")
        if  len(formula)==1:
            if formula[0]=='1':
                result.append(chooseRandom(vocabulary))
            elif formula[0]=='0':
                result.append([]) 
            else :
                arbitraries = vocabulary.copy()
                ap = formula[0]
                not_negated = True
                if ap[0]=='!':
                    ap = ap[1:]
                    
                    not_negated = False
                arbitraries.remove(ap)
                arbitraries = chooseRandom(arbitraries)
                if not_negated :
                    arbitraries.append(ap)
                result.append(arbitraries)
        elif formula[1]=='&' :
            i = 0
            set_vars = []
            arbitraries = vocabulary.copy()
            print(formula)
            while i < len(formula):
                ap = formula[i]
                if ap[0] == '!':
                    ap = ap[1:]
                else:
                    set_vars.append(ap)
                print(ap)
                arbitraries.remove(ap)
                i = i + 2
            trace_example = chooseRandom(arbitraries)
            trace_example.extend(set_vars)
            result.append(trace_example)
        else: 
#formula should be |
            rand = 2* random.randint(0,math.floor(len(formula)/2))
            arbitrary = vocabulary.copy()
            ap = formula[rand]
            not_negated = True
            if ap[0] == '!':
                ap = ap[1:]
                not_negated = False
            arbitrary.remove(ap)
            trace_example = chooseRandom(arbitrary)
            if (not_negated):
                trace_example.append(ap)
            result.append(trace_example)

    return result

def trace_to_file(trace, dirname, filename):
    f = open(dirname + "/" + filename, "w")
    for t in trace:
        s = "("
        for e in t:
            s = s + e + ","
        if (len(t) != 0):
            s = s[:-1]
        s = s + ")\n"
        f.write(s)
    f.write(";\n")
    f.close()



def main(argv):
    dirname = ''
    examplecountstring = ""
    ltl = ""
    mintracelengthstring= ""
    mintracelength = 1
    try: 
        opts, args = getopt.getopt(argv, "f:c:l:m:")
    except getopt.GetoptError:
        print ('trace_generator.py -f <foldername> -c <count>') 
        syst.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print ('trace_generator.py -f <foldername> -c <count> ')
            sys.exit()
        elif opt == '-f':
            dirname = arg
        elif opt == '-c':
            examplecountstring = arg
        elif opt == '-l':
            ltl = arg
        elif opt == '-m':
            mintracelengthstring = arg
            mintracelength = a=int(mintracelengthstring)
    examplecount = int(examplecountstring)
    atomic_props = ['a', 'b']
    if (ltl != ''):
        my_formula = spot.parse_formula(ltl)
    else :
        myseed = random.randint(0, 99999)
        f = spot.randltl(atomic_props, tree_size=10, seed=myseed, ltl_priorities='G=10, OR=10, M=0')
        my_formula = next(f)
    print (my_formula)
    formulas = []
    os.mkdir(dirname)
    for example in create_Examples(my_formula, examplecount, mintracelength):
        formulas.append(example.trace)
    count = 0
    while count < examplecount :
        current_trace = trace_to_variable(formulas[count], atomic_props)
        filename = "goodplan." + str(count)
        trace_to_file(current_trace, dirname, filename)
        count = count + 1

    my_formula = spot.formula.Not(my_formula)
    print (my_formula)
    formulas = []
    for example in create_Examples(my_formula, examplecount,mintracelength):
        formulas.append(example.trace)
    count = 0
    while count < examplecount :
        current_trace = trace_to_variable(formulas[count], atomic_props)
        filename = "badplan." + str(count)
        trace_to_file(current_trace, dirname, filename)
        count = count + 1

if __name__ == "__main__":
   main(sys.argv[1:])


