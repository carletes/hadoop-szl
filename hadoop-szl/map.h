//
// map.h
//

#ifndef _HADOOP_SZL_MAP_H
#define _HADOOP_SZL_MAP_H

#include <hadoop/Pipes.hh>


namespace sawzall {
class EmitterFactory;
class Executable;
class Process;
}


namespace hadoop_szl {

class Map : public HadoopPipes::Mapper {
  public:
    Map(HadoopPipes::TaskContext&);
    virtual ~Map();
    void map(HadoopPipes::MapContext&);

  private:
    void init(HadoopPipes::TaskContext&);

    sawzall::EmitterFactory* emitter_factory_;
    sawzall::Executable* exe_;
    sawzall::Process* process_;
    const std::string* szl_file_name_;
};

}

#endif
