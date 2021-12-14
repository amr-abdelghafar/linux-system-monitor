#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>
#include <sstream>

#include "linux_parser.h"


class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 long prev_idleJiffies = 0;
 long prev_activeJiffies = 0;
};

#endif