//
// map.h
//

#ifndef _HADOOP_SZL_MAP_H
#define _HADOOP_SZL_MAP_H

#include <hadoop/Pipes.hh>


namespace HadoopPipes {
class MapContext;
}

namespace sawzall {
class Executable;
class Process;
}


namespace hadoop_szl {

class Map : public HadoopPipes::Mapper {
  public:
    Map(HadoopPipes::TaskContext&);
    virtual ~Map();
    void map(HadoopPipes::MapContext&);

    const HadoopPipes::MapContext* context() const { return context_; }

  private:
    void init(HadoopPipes::TaskContext&);

    sawzall::Executable* exe_;
    sawzall::Process* process_;
    const std::string* szl_file_name_;
    HadoopPipes::MapContext* context_;
};

}

#endif
