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

struct Result {
    std::string name;
    std::string key;
    std::string value;
};

class Table {
  public:
    Table(const std::string& name, const std::string& type_string);

    bool Init(std::string* error);
    bool Add(const std::string& key, const std::string& value,
             std::string* error);
    void Flush();

    const std::vector<Result>* results() { return &results_; }

  private:
    std::string name_;
    std::string type_string_;
    SzlTabWriter* tab_writer_;
    std::map<std::string, SzlTabEntry*> tab_entries_;
    std::vector<Result> results_;
};

}

#endif /* _HADOOP_SZL_TABLE_H_ */
