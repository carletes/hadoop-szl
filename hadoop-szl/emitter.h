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

class Map;

class Emitter : public SzlEmitter {
  public:
    Emitter(const std::string& name, const SzlTabWriter* writer,
            const Map* mapper);
    virtual ~Emitter();

    static bool ParseNameKey(const std::string& line,
                             std::string* name,
                             std::string* key);
    static bool ParseValue(const std::string& line,
                           std::string* value);
    static bool Parse(const std::string& line,
                      std::string* name,
                      std::string* key, 
                      std::string* value);

  protected:
    virtual void WriteValue(const std::string& key, const std::string& value);
    void EncodeKey(const std::string& src, std::string* dest);
    void EncodeValue(const std::string& src, std::string* dest);

  private:
    const Map* mapper_;
};

class EmitterFactory : public sawzall::EmitterFactory {
  public:
    EmitterFactory(const Map* mapper);
    virtual ~EmitterFactory();

    sawzall::Emitter* NewEmitter(sawzall::TableInfo* table_info,
                                 std::string* error);

    void Flush();

  private:
    std::vector<SzlEmitter*> emitters_;
    const Map* mapper_;
};

}

#endif
