//
// reduce.cc
//

#include <hadoop-szl/reduce.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <hadoop-szl/emitter.h>
#include <hadoop-szl/runner.h>
#include <hadoop-szl/table.h>

#include <google/szl/porting.h>
#include <google/szl/logging.h>
#include <google/szl/szltabentry.h>


using std::cerr;
using std::endl;
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
        cerr << error << endl;
        throw runtime_error(error);
    }

    string name_key(context.getInputKey());
    string name, key, value;
    if (!Emitter::ParseNameKey(name_key, &name, &key)) {
        stringstream s;
        s << "Invalid key [" << name_key << "]";
        error = s.str();
        cerr << error << endl;
        cerr.flush();
        throw runtime_error(error);
    }

    Table* table = tables_[name];
    if (table == NULL) {
        stringstream s;
        s << name_key << ": Invalid table '" << name << "'";
        error = s.str();
        cerr << error << endl;
        cerr.flush();
        throw runtime_error(error);
    }

    while (context.nextValue()) {
        string enc_value(context.getInputValue());
        cerr << name_key << ": Processing value [" << enc_value << endl;
        cerr.flush();
        Emitter::ParseValue(enc_value, &value);
        if (!table->Add(key, value, &error)) {
            stringstream s;
            s << name_key << ": Error processing key [" << key <<
                 "], value [" << value << "]: " << error;
            string err(s.str());
            cerr << err << endl;
            cerr.flush();
            throw runtime_error(err);
        }
    }

    cerr << name_key << ": Collecting results" << endl;
    cerr.flush();

    table->Flush();
    const vector<Result>* res = table->results();
    for (unsigned int i = 0; i < res->size(); ++i) {
        Result r = (*res)[i];
        stringstream s;
        s << r.name << "[" << r.key << "]";
        context.emit(s.str(), r.value);
    }
}

}
