//
// map.cc
//

#include <hadoop-szl/map.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <hadoop/StringUtils.hh>

#include <google/szl/porting.h>
#include <google/szl/commandlineflags.h>
#include <google/szl/sawzall.h>
#include <google/szl/emitterinterface.h>


using std::cerr;
using std::endl;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

using HadoopPipes::MapContext;
using HadoopPipes::TaskContext;


namespace hadoop_szl {

class Emitter : public sawzall::Emitter {
  public:
    Emitter(const string& name)
      : name_(name) {}

    void Begin(GroupType type, int len) {
        cout << "[" << name_ << ": Begin type=" << type << ", len=" << len << "]" << endl;
    }

    void End(GroupType type, int len) {
        cout << "[" << name_ << ": End type=" << type << ", len=" << len << "]" << endl;
    }

    void PutBool(bool b) {
        cout << "[" << name_ << ": Bool " << b << "]" << endl;
    }

    void PutBytes(const char *p, int len) {
        cout << "[" << name_ << ": Bytes " << p << " (" << len << ")]" << endl;
    }

    void PutInt(int64 i) {
        cout << "[" << name_ << ": Int " << i << "]" << endl;
    }

    void PutFloat(double f) {
        cout << "[" << name_ << ": Float " << f << "]" << endl;
    }

    void PutFingerprint(uint64 fp) {
        cout << "[" << name_ << ": Fingerprint " << fp << "]" << endl;
    }

    void PutString(const char* s, int len) {
        cout << "[" << name_ << ": String " << s << " (" << len << ")]" << endl;
    }

    void PutTime(uint64 t) {
        cout << "[" << name_ << ": Time " << t << "]" << endl;
    }

    void EmitInt(int64 i) {
        cout << "[" << name_ << ": EmitInt " << i << "]" << endl;
    }

    void EmitFloat(double f) {
        cout << "[" << name_ << ": EmitFloat " << f << "]" << endl;
    }

  private:
    const string name_;
};


class EmitterFactory : public sawzall::EmitterFactory {
  public:
    EmitterFactory() {}

    ~EmitterFactory() {
        for (unsigned int i = 0; i < emitters_.size(); i++) {
            sawzall::Emitter* e = emitters_[i];
            delete e;
        }
    }

    sawzall::Emitter* NewEmitter(sawzall::TableInfo* table_info, string* error) {
        Emitter* e = new Emitter(table_info->name());
        emitters_.push_back(e);
        return e;
    }

    vector<sawzall::Emitter*>& emitters() { return emitters_; }

  private:
    vector<sawzall::Emitter*> emitters_;
};


Map::Map(TaskContext& context) {
    init(context);
}

Map::~Map() {
    delete szl_file_name_;
    delete process_;
    delete exe_;
    delete emitter_factory_;
}

void Map::init(TaskContext& context) {
    vector<string> cache_files =
        HadoopUtils::splitString(context.getJobConf()->get("mapred.cache.files"), ",");

    string szl_file(cache_files[1]);
    string::size_type i = szl_file.find('#');
    if (i == string::npos) {
        stringstream s;
        s << "Missing '#' in Sawzall file name " << szl_file;
        string err(s.str());
        cerr << err << endl;
        throw runtime_error(err);
    }
    szl_file_name_ = new string(szl_file.substr(i + 1));
    cerr << "Sawzall program: " << *szl_file_name_ << endl;

    exe_ = new sawzall::Executable(szl_file_name_->c_str(),
            NULL, sawzall::kNormal);
    if (!exe_->is_executable()) {
        stringstream s;
        s << *szl_file_name_ << ": Not a valid Sawzall program";
        string err(s.str());
        cerr << err << endl;
        throw runtime_error(err);
    }

    process_ = new sawzall::Process(exe_, false, NULL);
    emitter_factory_ = new EmitterFactory();
    process_->set_emitter_factory(emitter_factory_);

    //sawzall::RegisterEmitters(process_);

    if (!process_->Initialize()) {
        stringstream s;
        s << *szl_file_name_ << ": Cannot initialise Sawzall process";
        string err(s.str());
        cerr << err << endl;
        throw runtime_error(err);
    }
}

void Map::map(MapContext& context) {
    string input(context.getInputValue());
    if (!process_->Run(input.c_str(), strlen(input.c_str()), NULL, 0)) {
        cerr << *szl_file_name_ << ": Cannot run on input [" << input << "]" << endl;
    } else {
        // XXX Implement me!
    }
}

}
