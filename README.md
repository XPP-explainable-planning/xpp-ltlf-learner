1. Initialize all submodulse 

git submodule update --init --recursive

Build MONA/LTL2FOA

2.ltl2tgba should be in called, otherwise define path in learner/tracechecking/traceCheck.cpp
(We will use ltl2foa in future)

3. Build Learner
    cd learner && rm -r build && mkdir build && cd build && cmake .. && make
