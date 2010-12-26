//
// emitter.cc
//

#include <hadoop-szl/emitter.h>

#include <iostream>

#include <google/szl/logging.h>
#include <google/szl/szldecoder.h>
#include <google/szl/szltabentry.h>


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
    SzlDecoder key_decoder(key.data(), key.length());
    SzlDecoder value_decoder(value.data(), value.length());
    cout << "[" << name_ << ":" << key_decoder.PPrint() << "] [" << value_decoder.PPrint() << "]" << endl;
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
