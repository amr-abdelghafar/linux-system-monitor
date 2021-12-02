#ifndef PROCESSOR_H
#define PROCESSOR_H

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
  long int user_;
  long int nice_;
  long int system_;
  long int idle_;
  long int iowait_;
  long int irq_;
  long int softirq_;
  long int steal_;
  long int guest_;
  long int guest_nice_;
};

#endif