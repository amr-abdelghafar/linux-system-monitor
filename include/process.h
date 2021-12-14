#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  Process(int pid) : pid_(pid){
      user_     = LinuxParser::User(pid_);
      command_  = LinuxParser::Command(pid_);
      ram_      = LinuxParser::Ram(pid_);
      uptime_   = LinuxParser::UpTime(pid_);
      long total_time = LinuxParser::ActiveJiffies(pid_);
      long seconds = LinuxParser::UpTime() - uptime_;
      cpu_utilization_ = float(total_time) / float(seconds);
  }

  // TODO: Declare any necessary private members
 private:
 int pid_;
 std::string user_;
 std::string command_;
 float cpu_utilization_;
 std::string ram_;
 long int uptime_;

};

#endif