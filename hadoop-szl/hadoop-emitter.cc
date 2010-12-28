//
// hadoop-emitter.cc
//

#include <hadoop-szl/hadoop-emitter.h>

#include <iostream>

#include <hadoop-szl/map.h>


using std::cerr;
using std::endl;
using std::string;

using HadoopPipes::MapContext;


namespace hadoop_szl {

HadoopEmitter::HadoopEmitter(const string& name, const SzlTabWriter* writer,
                             const Map& mapper)
    : Emitter(name, writer), mapper_(mapper)
{
    cerr << "In HadoopEmitter::HadoopEmitter(" << name << ", " << writer << ")" << endl;
    cerr.flush();
}

void
HadoopEmitter::WriteValue(const string& key, const string& value)
{
    string enc_key, enc_value;
    cerr << "In HadoopEmitter::WriteValue()" << endl;
    cerr.flush();
    EncodeKey(key, &enc_key);
    EncodeValue(value, &enc_value);
    cerr << "Emitting [" << enc_key << "] [" << enc_value << "]" << endl;
    cerr.flush();
    MapContext* ctx = const_cast<MapContext *>(mapper_.context());
    cerr << "Map context: " << ctx << endl;
    cerr.flush();
    ctx->emit(enc_key, enc_value);
}

HadoopEmitterFactory::HadoopEmitterFactory(const Map& mapper)
    : EmitterFactory(), mapper_(mapper)
{
}

sawzall::Emitter*
HadoopEmitterFactory::NewEmitter(sawzall::TableInfo* table_info, string* error)
{
    Emitter* emitter = reinterpret_cast<Emitter *>(EmitterFactory::NewEmitter(table_info, error));
    if (emitter != NULL) {
        return new HadoopEmitter(emitter->name(), emitter->writer(), mapper_);
    } else {
        cerr << "Cannot create Hadoop emitter" << endl;
        cerr.flush();
        return NULL;
    }
}

}
