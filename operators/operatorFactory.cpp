#include "../definitions.hpp"

#include "operator.hpp"
#include <cassert>
#include "and.cpp"
#include "or.cpp"
#include "next.cpp"
#include "wnext.cpp"
#include "eventually.cpp"
#include "globally.cpp"
#include "until.cpp"
#include "release.cpp"
#include "globallyAnd.cpp"
#include "sequencedVisit.cpp"
#include "orderedSequencedVisit.cpp"
#include "omegaOperators/operatorTemplate.hpp"
#include "omegaOperators/metaOperator.cpp"

Operator* createOperator(int operatorType) {
    Operator* op;
    switch (operatorType) {
        case SkeletonType::lor:
            op = new OrOperator();
            return op;
        case SkeletonType::land:
            op = new AndOperator();
            return op;
        case SkeletonType::release:
            return (Operator*) new ReleaseOperator();
        case SkeletonType::until:
            return (Operator*) new UntilOperator();
        case SkeletonType::next:
            return (Operator*) new NextOperator();
        case SkeletonType::weaknext:
            return (Operator*) new WeakNextOperator();
        case SkeletonType::always:
            return (Operator*) new AlwaysOperator();
        case SkeletonType::eventually:
            return (Operator*) new EventuallyOperator();
        case 10: {

        }
        default:
            assert(false);
    }

}
