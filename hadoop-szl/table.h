//
// table.h
//

#ifndef _HADOOP_SZL_TABLE_H_
#define _HADOOP_SZL_TABLE_H_

#include <map>
#include <string>
#include <vector>


class SzlTabEntry;
class SzlTabWriter;


namespace hadoop_szl {

class Result {
  public:
      Result(const std::string& n, const std::string& k, const std::string& v)
          : name(n), key(k), value(v) {}
        
    const std::string name;
    const std::string key;
    const std::string value;
};

class Table {
  public:
    Table(const std::string& name, const std::string& type_string);
    virtual ~Table();

    bool Init(std::string* error);
    bool Add(const std::string& key, const std::string& value,
             std::string* error);
    std::vector<Result*>* Flush();

  private:
    std::string name_;
    std::string type_string_;
    SzlTabWriter* tab_writer_;
    std::map<std::string, SzlTabEntry*> tab_entries_;
    std::vector<Result*> results_;
};

}

#endif /* _HADOOP_SZL_TABLE_H_ */
