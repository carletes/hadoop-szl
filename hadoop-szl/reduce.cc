//
// reduce.cpp
//

#include <hadoop-szl/reduce.h>

#include <hadoop/StringUtils.hh>

using HadoopPipes::ReduceContext;

namespace hadoop_szl {

void Reduce::reduce(ReduceContext& context) {
    int sum = 0;

    while (context.nextValue()) {
        sum += HadoopUtils::toInt(context.getInputValue());
    }

    context.emit(context.getInputKey(), HadoopUtils::toString(sum));
}

}
