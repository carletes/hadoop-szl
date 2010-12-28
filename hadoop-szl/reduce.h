//
// reduce.h
//

#ifndef _HADOOP_SZL_REDUCE_H
#define _HADOOP_SZL_REDUCE_H

#include <map>

#include <hadoop/Pipes.hh>


namespace hadoop_szl {

class Table;

class Reduce : public HadoopPipes::Reducer {
  public:
    Reduce(HadoopPipes::TaskContext&) {}
    void reduce(HadoopPipes::ReduceContext&);

  private:
    std::map<std::string, Table*> tables_;
};

}

#endif
