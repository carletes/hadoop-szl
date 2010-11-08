//
// map.h
//

#ifndef _HADOOP_SZL_MAP_H
#define _HADOOP_SZL_MAP_H

#include <hadoop/Pipes.hh>

namespace hadoop_szl {

class Map : public HadoopPipes::Mapper {
public:
    Map(HadoopPipes::TaskContext&) {}
    void map(HadoopPipes::MapContext&);
};

}

#endif
