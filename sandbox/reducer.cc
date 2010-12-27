//
// reducer.cc
//

#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <google/szl/porting.h>
#include <google/szl/commandlineflags.h>
#include <google/szl/sawzall.h>
#include <google/szl/emitterinterface.h>
#include <google/szl/logging.h>
#include <google/szl/szltabentry.h>
#include <google/szl/szltype.h>

#include <hadoop-szl/emitter.h>
#include <hadoop-szl/table.h>


using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

using hadoop_szl::Result;
using hadoop_szl::Table;


int main(int argc, char **argv) {
    ProcessCommandLineArguments(argc, argv);
    InitializeAllModules();
    //sawzall::RegisterStandardTableTypes();

    sawzall::Executable exe(argv[1], NULL, sawzall::kNormal);
    if (!exe.is_executable()) {
        cerr << argv[1] << ": Not executable" << endl;
        return 1;
    }

    map<string, Table*> tables;
    string error;
    const vector<sawzall::TableInfo*>* tableinfo = exe.tableinfo();
    for (unsigned int i = 0; i < tableinfo->size(); ++i) {
        sawzall::TableInfo* t = (*tableinfo)[i];

        Table* table = new Table(t->name(), t->type_string());
        if (!table->Init(&error)) {
            cerr << "Invalid table '" << t->name() << "': " << error << endl;
            return 1;
        }
        tables[t->name()] = table;
    }

    string line;
    while (getline(cin, line)) {
        string name, key, value;
        if (!hadoop_szl::Emitter::Parse(line, &name, &key, &value)) {
            cerr << "Malformed line " << line << endl;
            continue;
        }

        Table* t = tables[name];
        if (t == NULL) {
            cerr << "Unknown table '" << name << "'" << endl;
            return 1;
        }
        if (!t->Add(key, value, &error)) {
            cerr << error << endl;
            return 1;
        }
    }

    map<string, Table*>::iterator pos;
    for (pos = tables.begin(); pos != tables.end(); ++pos) {
        Table* t = pos->second;
        t->Flush();
        const vector<Result>* res = t->results();
        for (unsigned int i = 0; i < res->size(); ++i) {
            Result r = (*res)[i];
            cout << r.name << "[" << r.key << "] = " << r.value << endl;
        }
    }
}
