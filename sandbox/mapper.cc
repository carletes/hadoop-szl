//
// mapper.cc
//

#include <cstring>
#include <iostream>
#include <vector>

#include <google/szl/porting.h>
#include <google/szl/logging.h>

#include <hadoop-szl/emitter.h>


using std::cerr;
using std::cout;
using std::endl;

using hadoop_szl::EmitterFactory;


const char *input[] = {
    "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do",
    "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut",
    "enim ad minim veniam, quis nostrud exercitation ullamco laboris",
    "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in",
    "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla",
    "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in",
    "culpa qui officia deserunt mollit anim id est laborum.",
    "Blah, blah."
};



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

    for (int i = 0; i < 8; i++) {
        if (!process.Run(input[i], strlen(input[i]), NULL, 0)) {
            cerr << argv[1] << ": Cannot execute" << endl;
            return 1;
        }
    }
    emitter_factory.Flush();
}
