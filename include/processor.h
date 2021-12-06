#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>
#include <sstream>

#include "linux_parser.h"


class Processor {
 public:
  /*
  Can be found from the /proc/stat
       user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
  cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
  */
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  struct cpu_data{
     std::string cpu_num;
     long int user;
     long int nice;
     long int system;
     long int idle;
     long int iowait;
     long int irq;
     long int softirq;
     long int steal;
     long int guest;
     long int guest_nice;
  };
  std::vector<cpu_data> system_cpus;
};

#endif