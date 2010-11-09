//
// map.cc
//

#include <hadoop-szl/map.h>

#include <string>
#include <vector>

#include <hadoop/StringUtils.hh>

using std::string;
using std::vector;

using HadoopPipes::MapContext;


namespace hadoop_szl {

void Map::map(MapContext& context) {
    vector<string> words = HadoopUtils::splitString(context.getInputValue(), " ");
    for(unsigned int i=0; i < words.size(); ++i) {
        context.emit(words[i], "1");
    }
}

}
