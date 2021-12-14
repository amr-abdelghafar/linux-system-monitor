#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization -- Done
float LinuxParser::MemoryUtilization() { 
  float used_memory_percent = 0.0, memtotal = 0.0, memfree = 0.0;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "MemTotal") linestream >> memtotal;
      if (key == "MemFree") linestream >> memfree;
    }
    used_memory_percent = ((memtotal - memfree) / memtotal);
  }
  return used_memory_percent; 
}

// TODO: Read and return the system uptime -- Done
long LinuxParser::UpTime() { 
  long uptime = 0, idletime = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long utime = 0, stime = 0, cutime = 0, cstime = 0, starttime = 0;
  string line, tmp;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 1; i < 23; i++){
      if(i == 14) linestream >> utime;
      else if(i == 15) linestream >> stime;
      else if(i == 16) linestream >> cutime;
      else if(i == 17) linestream >> cstime;
      else if(i == 22) linestream >> starttime;
      else linestream >> tmp;
    }
  }
  long total_time = utime + stime + cutime + cstime;
  return (total_time/sysconf(_SC_CLK_TCK));
}

// TODO: Read and return the number of active jiffies for the system

long LinuxParser::ActiveJiffies() {
  long active_jiffies = 0;
  vector<string> cpu_jiffies = LinuxParser::CpuUtilization();
  active_jiffies = (stol(cpu_jiffies[LinuxParser::CPUStates::kUser_])       -
                   stol(cpu_jiffies[LinuxParser::CPUStates::kGuest_]))      +
                   (stol(cpu_jiffies[LinuxParser::CPUStates::kNice_])       -
                   stol(cpu_jiffies[LinuxParser::CPUStates::kGuestNice_]))  +
                   stol(cpu_jiffies[LinuxParser::CPUStates::kSystem_])      +
                   stol(cpu_jiffies[LinuxParser::CPUStates::kIRQ_])         +
                   stol(cpu_jiffies[LinuxParser::CPUStates::kSoftIRQ_])     +
                   stol(cpu_jiffies[LinuxParser::CPUStates::kSteal_]);

  return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idle_jiffies = 0;
  vector<string> cpu_jiffies = LinuxParser::CpuUtilization();
  idle_jiffies = stol(cpu_jiffies[LinuxParser::CPUStates::kIdle_]) +
                 stol(cpu_jiffies[LinuxParser::CPUStates::kIOwait_]);

  return idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> jiffies;
  string cpu, jiffie, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while(linestream >> jiffie){
      jiffies.push_back(jiffie);
    }
  }
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalprocess = 0;
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
      while(std::getline(filestream, line)){
        std::istringstream linestream(line);
        linestream >> key;
        if (key == "processes") linestream >> totalprocess;
    }
  }
  return totalprocess;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runingprocess = 0;
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
      while(std::getline(filestream, line)){
        std::istringstream linestream(line);
        linestream >> key;
        if (key == "procs_running") linestream >> runingprocess;
    }
  }
  return runingprocess;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string ram;
  string key, line;
  long ram_num = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") { linestream >> ram_num;  break;}
    }
  }
  ram = to_string(ram_num/1000);
  return (ram);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid;
  string key, line;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") { linestream >> uid;  break;}
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user_name;
  string line, user, user_id, x;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> user_id;
      if (user_id == uid) { user_name = user;  break;}
    }
  }
  return user_name;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long uptime_sec = 0, uptime_clock_ticks_pid = 0;
  string line, tmp;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 1; i < 22; i++){
      linestream >> tmp;
    }
    linestream >> uptime_clock_ticks_pid;
    uptime_sec =  LinuxParser::UpTime() - (uptime_clock_ticks_pid / sysconf(_SC_CLK_TCK));
  }
  return uptime_sec;
}
