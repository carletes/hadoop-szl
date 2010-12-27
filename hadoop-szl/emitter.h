//
// emitter.h
//

#ifndef _HADOOP_SZL_EMITTER_H_
#define _HADOOP_SZL_EMITTER_H_

#include <string>

#include <google/szl/porting.h>
#include <google/szl/sawzall.h>
#include <google/szl/emitterinterface.h>
#include <google/szl/szlvalue.h>
#include <google/szl/szlemitter.h>


namespace hadoop_szl {

class Emitter : public SzlEmitter {
  public:
    Emitter(const std::string& name, const SzlTabWriter* writer);
    ~Emitter();

    static bool Parse(const std::string& line,
                      std::string* name,
                      std::string* key, 
                      std::string* value);

  private:
    virtual void WriteValue(const std::string& key, const std::string& value);
};

class EmitterFactory : public sawzall::EmitterFactory {
  public:
    EmitterFactory();
    ~EmitterFactory();

    sawzall::Emitter* NewEmitter(sawzall::TableInfo* table_info, std::string* error);

    void Flush();

  private:
    std::vector<SzlEmitter*> emitters_;
};

}

#endif
