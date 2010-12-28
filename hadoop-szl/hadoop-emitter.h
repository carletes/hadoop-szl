//
// hadoop-emitter.cc
//

#ifndef _HADOOP_SZL_HADOOP_EMITTER_H_
#define _HADOOP_SZL_HADOOP_EMITTER_H_

#include <string>

#include <hadoop-szl/emitter.h>


class SzlTabWriter;

namespace sawzall {
class Emitter;
class TableInfo;
}


namespace hadoop_szl {

class Map;

class HadoopEmitter : public Emitter {
  public:
    HadoopEmitter(const std::string& name, const SzlTabWriter* writer,
                  const Map& mapper);

  protected:
    virtual void WriteValue(const std::string& key, const std::string& value);

  private:
    const Map& mapper_;
};


class HadoopEmitterFactory : public EmitterFactory {
  public:
    HadoopEmitterFactory(const Map& mapper);

    sawzall::Emitter* NewEmitter(sawzall::TableInfo* table_info,
                                 std::string* error);

  private:
    const Map& mapper_;
};

}

#endif /* _HADOOP_SZL_HADOOP_EMITTER_H_ */
