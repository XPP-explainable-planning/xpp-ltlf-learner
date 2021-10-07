cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3")
project(ltlflearner-sat LANGUAGES CXX)


file(GLOB OPERATORS
    "operators/*.hpp"
    "operators/*.cpp"
    "operators/omegaOperators/*.hpp"
    "operators/omegaOperators/*.cpp"
    "traceChecking/*.hpp"
    "traceChecking/*.cpp"
)

add_executable(ltlf_learner main.cpp cnf.hpp cnf.cpp parser.cpp variable.hpp varsDict.hpp varsDict.cpp definitions.hpp parser.hpp ${OPERATORS}) 


add_subdirectory(lib/minisat)
target_link_libraries(ltlf_learner MiniSat::libminisat)

