//
// hadoop-szl-runner.cc
//

#include <hadoop/Pipes.hh>
#include <hadoop/TemplateFactory.hh>

#include <hadoop-szl/map.h>
#include <hadoop-szl/reduce.h>

#include <google/szl/porting.h>
#include <google/szl/commandlineflags.h>
#include <google/szl/sawzall.h>


using HadoopPipes::runTask;
using HadoopPipes::TemplateFactory;


int main(int argc, char *argv[]) {
    InitializeAllModules();
    //sawzall::RegisterStandardTableTypes();

    return runTask(TemplateFactory<hadoop_szl::Map, hadoop_szl::Reduce>());
}
