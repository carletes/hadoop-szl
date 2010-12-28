//
// runner.h
//

#ifndef _HADOOP_SZL_RUNNER_H_
#define _HADOOP_SZL_RUNNER_H_

#include <string>


namespace HadoopPipes {
class TaskContext;
}

namespace sawzall {
class Executable;
}

namespace hadoop_szl {

class SawzallRunner {
  public:
    SawzallRunner(HadoopPipes::TaskContext& context);
    virtual ~SawzallRunner();

    virtual bool Init(std::string* error);

    const sawzall::Executable* exe() const { return exe_; }

  protected:
    std::string* szl_file_name_;
    sawzall::Executable* exe_;

  private:
    HadoopPipes::TaskContext& context_;
    bool initialized_;
};

}

#endif /* _HADOOP_SZL_RUNNER_H_ */
