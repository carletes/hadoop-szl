//
// table.cc
//

#include <hadoop-szl/table.h>

#include <sstream>

#include <google/szl/porting.h>
#include <google/szl/logging.h>
#include <google/szl/szldecoder.h>
#include <google/szl/szltabentry.h>
#include <google/szl/szltype.h>


using std::string;
using std::stringstream;
using std::vector;


namespace hadoop_szl {

Table::Table(const string& name, const string& type_string)
    : name_(name), type_string_(type_string)
{
}

bool
Table::Init(string* error)
{
    SzlType t(SzlType::VOID);
        
    if (!t.ParseFromSzlArray(type_string_.data(),
                             type_string_.size(),
                             error)) {
        return false;
    }
        
    if ((tab_writer_ = SzlTabWriter::CreateSzlTabWriter(t, error)) == NULL) {
        return false;
    }

    return true;
}

bool
Table::Add(const string& key, const string& value, string* error)
{
    SzlTabEntry* te = tab_entries_[key];
    if (te == NULL) {
        te = tab_writer_->CreateEntry(key);
        if (te == NULL) {
            *error = "Out of memory";
            return false;
        }
        tab_entries_[key] = te;
    }

    if (tab_writer_->Aggregates()) {
        if (te->Merge(value) == SzlTabEntry::MergeError) {
            stringstream s;
            s << name_ << ": Cannot merge result [" << value << "]";
            *error = s.str();
            return false;
        }
    } else {
        if (tab_writer_->WritesToMill()) {
            Result res;
            res.name = name_;
            SzlDecoder key_decoder(key.data(), key.length());
            res.key = key_decoder.PPrint();
            SzlDecoder value_decoder(value.data(), value.length());
            res.value = value_decoder.PPrint();
            results_.push_back(res);
        } else {
            te->Write(value);
        }
    }

    return true;
}

void
Table::Flush()
{
    if (tab_writer_->Aggregates()) {
        map<string, SzlTabEntry*>::iterator pos;
        for (pos = tab_entries_.begin(); pos != tab_entries_.end(); ++pos) {
            Result res;
            res.name = name_;
            string key(pos->first);
            SzlDecoder key_decoder(key.data(), key.length());
            res.key = key_decoder.PPrint();
            SzlTabEntry* te = pos->second;
            string value;
            te->Flush(&value);
            SzlDecoder value_decoder(value.data(), value.length());
            res.value = value_decoder.PPrint();
            results_.push_back(res);
        }
    }
}

}
