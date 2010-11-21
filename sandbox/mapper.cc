//
// mapper.cc
//

#include <cstring>
#include <iostream>
#include <vector>

#include <google/szl/porting.h>
#include <google/szl/commandlineflags.h>
#include <google/szl/sawzall.h>
#include <google/szl/emitterinterface.h>


using std::cerr;
using std::cout;
using std::endl;


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
        for (int i = 0; i < emitters_.size(); i++) {
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



char *input[] = {
    "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do",
    "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut",
    "enim ad minim veniam, quis nostrud exercitation ullamco laboris",
    "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in",
    "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla",
    "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in",
    "culpa qui officia deserunt mollit anim id est laborum."
};



int main(int argc, char **argv) {
    InitializeAllModules();
    //sawzall::RegisterStandardTableTypes();

    sawzall::Executable exe(argv[1], NULL, sawzall::kNormal);
    if (!exe.is_executable()) {
        cerr << argv[1] << ": Not executable" << endl;
        return 1;
    }

    sawzall::Process process(&exe, false, NULL);
    EmitterFactory emitter_factory;
    process.set_emitter_factory(&emitter_factory);
    //sawzall::RegisterEmitters(&process);

    if (!process.Initialize()) {
        cerr << argv[1] << ": Cannot initialise" << endl;
        return 1;
    }

    for (int i = 0; i < 7; i++) {
        cout << "Record " << i << endl;
        if (!process.Run(input[i], strlen(input[i]), NULL, 0)) {
            cerr << argv[1] << ": Cannot execute" << endl;
            return 1;
        }
    }
}
