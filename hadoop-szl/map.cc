//
// map.cc
//

#include <hadoop-szl/map.h>

#include <cstring>
#include <sstream>
#include <string>

#include <hadoop-szl/runner.h>

#include <google/szl/porting.h>
#include <google/szl/sawzall.h>

#include <hadoop-szl/emitter.h>


using std::stringstream;

using HadoopPipes::MapContext;
using HadoopPipes::TaskContext;

using sawzall::Process;


namespace hadoop_szl {

SawzallMapRunner::SawzallMapRunner(TaskContext& context, Map& mapper)
    : SawzallRunner(context), mapper_(mapper), process_(NULL),
      emitter_factory_(NULL), initialized_(false)
{
}

SawzallMapRunner::~SawzallMapRunner()
{
    delete process_;
    delete emitter_factory_;
}

bool
SawzallMapRunner::Init(string* error)
{
    if (initialized_) {
        return true;
    }

    if (!SawzallRunner::Init(error)) {
        return false;
    }

    if (process_ == NULL) {
        process_ = new Process(exe_, false, NULL);
        emitter_factory_ = new EmitterFactory(&mapper_);
        process_->set_emitter_factory(emitter_factory_);
    }

    if (!process_->Initialize()) {
        stringstream s;
        s << *szl_file_name_ << ": Cannot initialize Sawzall process";
        *error = s.str();
        return false;
    }
    initialized_ = true;
    return true;
}

bool
SawzallMapRunner::Run(MapContext& context, const string& input)
{
    mapper_.context = &context;
    if (!process_->Run(input.c_str(), strlen(input.c_str()), NULL, 0)) {
        return false;
    }
    emitter_factory_->Flush();
    return true;
}

Map::Map(TaskContext& context)
{
    szl_ = new SawzallMapRunner(context, *this);
}

Map::~Map()
{
    delete szl_;
}

void Map::map(MapContext& context) {
    string error;
    if (!szl_->Init(&error)) {
        stringstream s;
        s << "Error initializing Sawzall: " << error;
        context.setStatus(s.str());
        return;
    }

    string input(context.getInputValue());
    context.setStatus("Processing input");
    if (!szl_->Run(context, input)) {
        stringstream s;
        s << "Sawzalls failled on input [" << input << "]";
        context.setStatus(s.str());
    } else {
        context.progress();
    }
}

}
