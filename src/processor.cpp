#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

    float total_CpuTime = LinuxParser::Jiffies();
    float idle_CpuTime = LinuxParser::IdleJiffies();

    float delta_idle = idle_CpuTime - pre_idle_CpuTime;
    float delta_total = total_CpuTime - pre_total_CpuTime;
    float delta_usage = (delta_total - delta_idle)/ delta_total;

    pre_idle_CpuTime = idle_CpuTime;
    pre_total_CpuTime = total_CpuTime;

    return delta_usage;


}//{ return 0.0; }