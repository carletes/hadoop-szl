//
// runner.cc
//

#include <hadoop-szl/runner.h>

#include <sstream>
#include <vector>

#include <hadoop/Pipes.hh>
#include <hadoop/StringUtils.hh>

#include <google/szl/porting.h>
#include <google/szl/sawzall.h>


using std::string;
using std::stringstream;
using std::vector;

using HadoopPipes::MapContext;
using HadoopPipes::TaskContext;

using sawzall::Executable;
using sawzall::Process;


namespace hadoop_szl {

SawzallRunner::SawzallRunner(TaskContext& context)
    : szl_file_name_(NULL), exe_(NULL), context_(context), initialized_(false)
{
}

SawzallRunner::~SawzallRunner()
{
    delete szl_file_name_;
    delete exe_;
}

bool
SawzallRunner::Init(string *error)
{
    if (initialized_) {
        return true;
    }

    if (szl_file_name_ == NULL) {
        vector<string> cache_files =
            HadoopUtils::splitString(context_.getJobConf()->get("mapred.cache.files"), ",");

        string szl_file(cache_files[1]);
        string::size_type i = szl_file.find('#');
        if (i == string::npos) {
            stringstream s;
            s << "Missing '#' in Sawzall file name " << szl_file;
            *error = s.str();
            return false;
        }
        szl_file_name_ = new string(szl_file.substr(i + 1));
    }

    if (exe_ == NULL) {
        exe_ = new Executable(szl_file_name_->c_str(), NULL, sawzall::kNormal);
        if (!exe_->is_executable()) {
            stringstream s;
            s << *szl_file_name_ << ": Not a valid Sawzall program";
            *error = s.str();
            return false;
        }
    }

    initialized_ = true;
    return true;
}

}
