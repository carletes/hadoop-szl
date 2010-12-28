//
// reduce.cc
//

#include <hadoop-szl/reduce.h>

#include <sstream>
#include <stdexcept>
#include <vector>

#include <hadoop-szl/emitter.h>
#include <hadoop-szl/runner.h>
#include <hadoop-szl/table.h>

#include <google/szl/porting.h>
#include <google/szl/logging.h>
#include <google/szl/szltabentry.h>


using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

using HadoopPipes::ReduceContext;
using HadoopPipes::TaskContext;

using hadoop_szl::Emitter;
using hadoop_szl::Result;
using hadoop_szl::SawzallRunner;
using hadoop_szl::Table;


namespace hadoop_szl {

Reduce::Reduce(TaskContext& context)
    : initialized_(false)
{
    szl_ = new SawzallRunner(context);
}

Reduce::~Reduce()
{
    delete szl_;
}

bool
Reduce::Init(string* error)
{
    if (initialized_) {
        return true;
    }

    if (!szl_->Init(error)) {
        return false;
    }

    const vector<sawzall::TableInfo*>* tableinfo = szl_->exe()->tableinfo();
    for (unsigned int i = 0; i < tableinfo->size(); ++i) {
        sawzall::TableInfo* t = (*tableinfo)[i];

        Table* table = new Table(t->name(), t->type_string());
        if (!table->Init(error)) {
            return false;
        }
        tables_[t->name()] = table;
    }

    initialized_ = true;
    return true;
}

void
Reduce::reduce(ReduceContext& context)
{
    string error;
    if (!Init(&error)) {
        throw runtime_error(error);
    }

    string name_key(context.getInputKey());
    string name, key, value;
    if (!Emitter::ParseNameKey(name_key, &name, &key)) {
        stringstream s;
        s << "Invalid key [" << name_key << "]";
        throw runtime_error(s.str());
    }

    Table* table = tables_[name];
    if (table == NULL) {
        stringstream s;
        s << name_key << ": Invalid table '" << name << "'";
        throw runtime_error(s.str());
    }

    while (context.nextValue()) {
        string enc_value(context.getInputValue());
        Emitter::ParseValue(enc_value, &value);
        if (!table->Add(key, value, &error)) {
            stringstream s;
            s << name_key << ": Error processing key [" << key <<
                 "], value [" << value << "]: " << error;
            throw runtime_error(s.str());
        }
    }

    table->Flush();
    const vector<Result>* res = table->results();
    for (unsigned int i = 0; i < res->size(); ++i) {
        Result r = (*res)[i];
        stringstream s;
        s << r.name << "[" << r.key << "]";
        context.emit(s.str(), r.value);
    }

    context.progress();
}

}
