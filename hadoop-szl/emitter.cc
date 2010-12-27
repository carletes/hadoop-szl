//
// emitter.cc
//

#include <hadoop-szl/emitter.h>

#include <iostream>

#include <google/szl/logging.h>
#include <google/szl/szldecoder.h>
#include <google/szl/szltabentry.h>

#include <hadoop-szl/base64.h>


using std::cout;
using std::endl;
using std::string;


namespace hadoop_szl {

Emitter::Emitter(const string& name, const SzlTabWriter* writer)
    : SzlEmitter(name, writer, false)
{
}

Emitter::~Emitter()
{
}

void
Emitter::WriteValue(const string& key, const string& value)
{
    string enc_key, enc_value;

    if (key.size()) {
        Base64Encode(key, &enc_key);
    } else {
        enc_key = "*";
    }

    Base64Encode(value, &enc_value);
    cout << name_ << " " << enc_key << " " << enc_value << endl;
}

bool
Emitter::Parse(const string& line, string* name, string* key, string* value)
{
    string line2 = line;
    string::size_type sep = line2.find(" ");
    if (sep == line2.npos) {
        return false;
    }
    *name = line2.substr(0, sep);
    line2 = line2.substr(sep + 1);

    sep = line2.find(" ");
    if (sep == line2.npos) {
        return false;
    }
    string enc_key(line2.substr(0, sep));
    if (enc_key == "*") {
        *key = "";
    } else {
        Base64Decode(enc_key, key);
    }
    line2 = line2.substr(sep + 1);

    Base64Decode(line2, value);
    return true;
}

EmitterFactory::EmitterFactory()
{
}

EmitterFactory::~EmitterFactory()
{
    for (unsigned int i = 0; i < emitters_.size(); ++i) {
        sawzall::Emitter* e = emitters_[i];
        delete e;
    }
}

sawzall::Emitter*
EmitterFactory::NewEmitter(sawzall::TableInfo* table_info, string* error)
{
    SzlEmitter* emitter = NULL;

    string type_string(table_info->type_string());
    SzlType type(SzlType::VOID);
    if (type.ParseFromSzlArray(type_string.data(), type_string.size(),
                               error)) {
        SzlTabWriter* tw = SzlTabWriter::CreateSzlTabWriter(type, error);
        if (tw != NULL) {
            emitter = new Emitter(table_info->name(), tw);

            cerr << "New emitter for '" << table_info->name() << "'" << endl;
        }
    }
    emitters_.push_back(emitter);
    return emitter;
}

void
EmitterFactory::Flush()
{
    for (unsigned int i = 0; i < emitters_.size(); ++i) {
        emitters_[i]->Flusher();
    }
}

}
