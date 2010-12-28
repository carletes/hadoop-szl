//
// reduce.h
//

#ifndef _HADOOP_SZL_REDUCE_H
#define _HADOOP_SZL_REDUCE_H

#include <map>
#include <string>

#include <hadoop/Pipes.hh>


namespace hadoop_szl {

class SawzallRunner;
class Table;


class Reduce : public HadoopPipes::Reducer {
  public:
    Reduce(HadoopPipes::TaskContext& context);
    virtual ~Reduce();

    bool Init(std::string* error);

    void reduce(HadoopPipes::ReduceContext& context);

  private:
    SawzallRunner* szl_;
    std::map<std::string, Table*> tables_;
    bool initialized_;
};

}

#endif
