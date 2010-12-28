//
// map.h
//

#ifndef _HADOOP_SZL_MAP_H
#define _HADOOP_SZL_MAP_H

#include <hadoop/Pipes.hh>

#include <hadoop-szl/runner.h>


namespace sawzall {
class Process;
}


namespace hadoop_szl {

class EmitterFactory;
class Map;


class SawzallMapRunner : SawzallRunner {
  public:
    SawzallMapRunner(HadoopPipes::TaskContext& context, Map& mapper);
    virtual ~SawzallMapRunner();

    virtual bool Init(std::string* error);
    bool Run(HadoopPipes::MapContext& context, const std::string& input);

  private:
    Map& mapper_;
    sawzall::Process* process_;
    EmitterFactory* emitter_factory_;
    bool initialized_;
};


class Map : public HadoopPipes::Mapper {
  public:
    Map(HadoopPipes::TaskContext&);
    virtual ~Map();
    void map(HadoopPipes::MapContext&);

    HadoopPipes::MapContext* context;

  private:
    SawzallMapRunner* szl_;
};

}

#endif
