//
// emitter.cc
//

#include <hadoop-szl/emitter.h>

#include <iostream>
#include <sstream>

#include <google/szl/logging.h>
#include <google/szl/szldecoder.h>
#include <google/szl/szltabentry.h>

#include <hadoop-szl/base64.h>


using std::cout;
using std::endl;
using std::string;
using std::stringstream;


namespace hadoop_szl {

Emitter::Emitter(const string& name, const SzlTabWriter* writer)
    : SzlEmitter(name, writer, false)
{
}

Emitter::~Emitter()
{
}

void
Emitter::EncodeKey(const string& src, string* dest)
{
    stringstream s;

    s << name_;
    s << ":";
    if (src.size()) {
        string k;
        Base64Encode(src, &k);
        s << k;
    } else {
        s << "*";
    }
    *dest = s.str();
}

void
Emitter::EncodeValue(const string& src, string* dest)
{
    Base64Encode(src, dest);
}

void
Emitter::WriteValue(const string& key, const string& value)
{
    string enc_key, enc_value;
    EncodeKey(key, &enc_key);
    EncodeValue(value, &enc_value);
    cout << enc_key << " " << enc_value << endl;
}

bool
Emitter::ParseNameKey(const string& line, string* name, string* key)
{
    string::size_type sep = line.find(":");
    if (sep == line.npos) {
        return false;
    }
    *name = line.substr(0, sep);

    string enc_key(line.substr(sep + 1));
    if (enc_key == "*") {
        *key = "";
    } else {
        Base64Decode(enc_key, key);
    }
    return true;
}

bool
Emitter::ParseValue(const string& line, string* value)
{
    Base64Decode(line, value);
    return true;
}

bool
Emitter::Parse(const string& line, string* name, string* key, string* value)
{
    string::size_type sep = line.find(" ");
    if (sep == line.npos) {
        return false;
    }
    if (!ParseNameKey(line.substr(0, sep), name, key)) {
        return false;
    }
    return ParseValue(line.substr(sep + 1), value);
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
