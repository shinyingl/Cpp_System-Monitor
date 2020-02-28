#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
#include <fstream>

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
    linestream >> os >> version>> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value, kb;
  float MemTotal = 0.0;
  float MemFree = 0.0;
  std::ifstream stream(kProcDirectory+ kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >>kb) {
        if (key == "MemTotal") {MemTotal = std::stof(value); }
          else if (key == "MemFree") {MemFree = std::stof(value);}
      }
    }
  }
  return (MemTotal - MemFree)/MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long long_uptime = 0;
  string value;
  string idletime;
  string line;
  float float_uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value >> idletime) {
      std::ofstream myf;
      myf.open("out.txt");
      float_uptime = std::stof(value);
      myf << value << " "<< idletime << " " << float_uptime << "\n";
      myf.close();
      
    }
  }
  return (long)float_uptime;
}


//https://supportcenter.checkpoint.com/supportcenter/portal?eventSubmit_doGoviewsolutiondetails=&solutionid=sk65143

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {//{ return 0; }
  
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();

}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {//{ return 0; }
  string value, line;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      stat_list.push_back(value);
    }
  }
  long int utime{std::stol(stat_list[13])};
  long int stime{std::stol(stat_list[14])};
  long int cutime{std::stol(stat_list[15])};
  long int cstime{std::stol(stat_list[16])};

  return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { //{ return 0; }
  long systemtime, irq, soft_irq;
  string value1, value2, value3, value4, value5, value6, value7, line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> key >> value1 >> value2 >> value3 >> value4 >> value5 >> value6 >> value7;
      if (key == "cpu") {
        //printf("Helooo:  %s %s\n", key, value3);
        systemtime = std::stol(value3);
        irq = std::stol(value6);
        soft_irq = std::stol(value7);
        return systemtime + irq + soft_irq;

      }
    
  
  }
  return 0;
  
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {//{ return 0; }
  string value1, value2, value3, value4, value5, line, key;
  long idletime, iowait;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value1 >> value2 >> value3 >> value4 >> value5; 
      if (key == "cpu") {
        idletime = std::stol(value4);
        iowait = std::stol(value5);
        return idletime + iowait;

      }
    
  
  }
 return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()  { return {}; } // NOT USED?
// {
//   std::ifstream cpustream(kProcDirectory + kStatFilename);
//   if (cpustream.is_open()) {
//     std::getline(cpustream, line);
//     std::istringstream linestream(line);
//     while (linestream >>key) {
//       if (key !="cpu") {
//         cpu_value.push_back(key);

//       }
//     }
  
//   }
//   return cpu_value;
// }


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {//{ return 0; }
  int total_pro;
  string value, key, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >>key >>value) {
        if (key =="processes") {
          total_pro = std::stoi(value);
          return total_pro;

        }
      }
    }
   

  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {//{ return 0; }
  int running_p;
  string value, key, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >>key >> value) {
        if (key =="procs_running") {
          running_p = std::stoi(value);
          return running_p;

        }
      }
    }
   

  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {//{ return string(); }
  string cmd, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >>cmd) {
        return cmd;

      }
    }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {//{ return string(); }
  long vmsize;
  string value, kb, key, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
      while (linestream >>key >> value >> kb) {
        if (key == "VmSize:") {
          vmsize = std::stol(value)/1000; //convert to MB
          return std::to_string(vmsize);
        }
      }

    }
  }
   
  return string();

}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {//{ return string(); }

  string value;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
      while (linestream >>key >> value ) {
        if (key == "Uid:") {
          
          return value;
        }
      }

    }

  }
  return value; 
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {//{ return string(); }
  string value, passwd, key, line;
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> value >> passwd >> key) {
          if (key == uid) {return value;}
          
        }
      }

  }
  return value;

}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {//{ return 0; }
  string value, line;
  long int starttime, uptime;
  vector<string> statlist;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
    while (linestream >> value) {
      statlist.push_back(value);
    }
    starttime = std::stol(statlist[21])/sysconf(_SC_CLK_TCK);
    uptime = LinuxParser::UpTime() - starttime;
    return uptime;

  }
  return 0;
}

//https://unix.stackexchange.com/questions/62154/when-was-a-process-started