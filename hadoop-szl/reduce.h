//
// reduce.h
//

#ifndef _HADOOP_SZL_REDUCE_H
#define _HADOOP_SZL_REDUCE_H

#include <hadoop/Pipes.hh>

namespace hadoop_szl {

class Reduce : public HadoopPipes::Reducer {
public:
    Reduce(HadoopPipes::TaskContext&) {}
    void reduce(HadoopPipes::ReduceContext&);
};

}

#endif
