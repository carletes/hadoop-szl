//
// mapper.cc
//

#include <fstream>
#include <iostream>
#include <vector>

#include <google/szl/porting.h>
#include <google/szl/logging.h>

#include <hadoop-szl/emitter.h>


using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;

using hadoop_szl::EmitterFactory;


int main(int argc, char **argv) {
    InitializeAllModules();
    sawzall::RegisterStandardTableTypes();

    sawzall::Executable exe(argv[1], NULL, sawzall::kNormal);
    if (!exe.is_executable()) {
        cerr << argv[1] << ": Not executable" << endl;
        return 1;
    }

    sawzall::Process process(&exe, false, NULL);
    EmitterFactory emitter_factory;
    process.set_emitter_factory(&emitter_factory);
    sawzall::RegisterEmitters(&process);

    if (!process.Initialize()) {
        cerr << argv[1] << ": Cannot initialise" << endl;
        return 1;
    }

    ifstream in(argv[2]);
    if (!in) {
        cerr << argv[1] << ": Cannot open " << argv[2] << endl;
        return 1;
    }

    int rc = 0;
    string line;
    while (getline(in, line)) {
        if (!process.Run(line.data(), line.size(), NULL, 0)) {
            cerr << argv[1] << ": Cannot process [" << line << "]" << endl;
            rc = 1;
        }
    }
    emitter_factory.Flush();
    return rc;
}
