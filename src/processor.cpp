#include "processor.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
    // user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
//cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
float Processor::Utilization() {
    float btime = 0;
    string key;
    float total_utilization = 0.0;
    vector<string> cpus = LinuxParser::CpuUtilization();
    for(string cpu: cpus){
        std::istringstream linestream(cpu);
        linestream >> key;
        if (key == "btime"){
            linestream >> btime;
        }else{
        cpu_data cpu_d;
         cpu_d.cpu_num = key;
        linestream >> cpu_d.user >> cpu_d.nice >> cpu_d.system
                   >> cpu_d.idle >> cpu_d.iowait >> cpu_d.irq >> cpu_d.softirq
                   >> cpu_d.steal >> cpu_d.guest >> cpu_d.guest_nice;
        system_cpus.push_back(cpu_d);
        }
    }
    cpu_data cpu_d = {system_cpus[0]};
    total_utilization = (cpu_d.user + cpu_d.nice + cpu_d.system
                        + cpu_d.idle + cpu_d.iowait + cpu_d.irq + cpu_d.softirq
                        + cpu_d.steal + cpu_d.guest + cpu_d.guest_nice)
                        / btime;
    return total_utilization;
}