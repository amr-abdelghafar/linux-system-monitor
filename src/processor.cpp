#include "processor.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

    long active = LinuxParser::ActiveJiffies();
    long idle = LinuxParser::IdleJiffies();
    long long total = active + idle;

    long prev_active = prev_activeJiffies;
    long prev_idle = prev_idleJiffies;
    long long prev_total = prev_active + prev_idle;

    prev_activeJiffies = active;
    prev_idleJiffies = idle;

    // differentiate: actual value minus the previous one
    float total_delta = total - prev_total;
    float idle_delta = idle - prev_idle;
    float cpu_utilization = (total_delta - idle_delta) / total_delta;
    return cpu_utilization;
}