//
// reducer.cc
//

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <google/szl/porting.h>
#include <google/szl/commandlineflags.h>
#include <google/szl/sawzall.h>
#include <google/szl/emitterinterface.h>
#include <google/szl/logging.h>
#include <google/szl/szltabentry.h>
#include <google/szl/szltype.h>


using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;


int main(int argc, char **argv) {
    ProcessCommandLineArguments(argc, argv);
    InitializeAllModules();
    //sawzall::RegisterStandardTableTypes();

    sawzall::Executable exe(argv[1], NULL, sawzall::kNormal);
    if (!exe.is_executable()) {
        cerr << argv[1] << ": Not executable" << endl;
        return 1;
    }

    sawzall::PrintUniverse();
    cout << endl;

    exe.PrintTables();
    cout << endl;

    const vector<sawzall::TableInfo*>* tableinfo = exe.tableinfo();
    for (unsigned int i = 0; i < tableinfo->size(); ++i) {
        sawzall::TableInfo* t = (*tableinfo)[i];

        SzlType type(SzlType::VOID);
        string type_string(t->type_string());
        string error;
        
        if (!type.ParseFromSzlArray(type_string.data(),
                                    type_string.size(),
                                    &error)) {
            cerr << "Invalid table '" << t->name() << "': " << error << endl;
            return 1;
        }
        
        if (SzlTabWriter::CreateSzlTabWriter(type, &error) == NULL) {
            cerr << "Cannot create tab writer: " << error << endl;
            return 1;
        }
    }
}
