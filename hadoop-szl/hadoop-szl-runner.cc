//
// hadoop-szl-runner.cc
//

#include <hadoop/Pipes.hh>
#include <hadoop/TemplateFactory.hh>

#include <hadoop-szl/map.h>
#include <hadoop-szl/reduce.h>

using HadoopPipes::runTask;
using HadoopPipes::TemplateFactory;


int main(int argc, char *argv[]) {
    return runTask(TemplateFactory<hadoop_szl::Map, hadoop_szl::Reduce>());
}
