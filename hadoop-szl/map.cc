//
// map.cc
//

#include <hadoop-szl/map.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <hadoop/StringUtils.hh>

#include <google/szl/porting.h>
#include <google/szl/commandlineflags.h>
#include <google/szl/sawzall.h>

#include <hadoop-szl/hadoop-emitter.h>


using std::cerr;
using std::endl;
using std::runtime_error;
using std::stringstream;

using sawzall::Executable;
using sawzall::Process;

using HadoopPipes::MapContext;
using HadoopPipes::TaskContext;


namespace hadoop_szl {

Map::Map(TaskContext& context) {
    init(context);
}

Map::~Map() {
    delete szl_file_name_;
    delete process_;
    delete exe_;
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
        cerr.flush();
        throw runtime_error(err);
    }
    szl_file_name_ = new string(szl_file.substr(i + 1));
    cerr << "Sawzall program: " << *szl_file_name_ << endl;

    exe_ = new Executable(szl_file_name_->c_str(), NULL, sawzall::kNormal);
    if (!exe_->is_executable()) {
        stringstream s;
        s << *szl_file_name_ << ": Not a valid Sawzall program";
        string err(s.str());
        cerr << err << endl;
        cerr.flush();
        throw runtime_error(err);
    }

    process_ = new Process(exe_, false, NULL);
    HadoopEmitterFactory emitter_factory(*this);
    process_->set_emitter_factory(&emitter_factory);

    if (!process_->Initialize()) {
        stringstream s;
        s << *szl_file_name_ << ": Cannot initialise Sawzall process";
        string err(s.str());
        cerr << err << endl;
        cerr.flush();
        throw runtime_error(err);
    }
}

void Map::map(MapContext& context) {
    string input(context.getInputValue());
    cerr << "Processing input [" << input << "]" << endl;
    cerr.flush();
    context_ = &context;
    if (!process_->Run(input.c_str(), strlen(input.c_str()), NULL, 0)) {
        cerr << *szl_file_name_ << ": Cannot run on input [" << input << "]" << endl;
        cerr.flush();
    } else {
        cerr << "Done processing input [" << input << "]" << endl;
        cerr.flush();
    }
}

}
