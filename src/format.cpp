#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long HH{seconds/3600};
    long MM{(seconds%3600)/60};
    long SS{(seconds%3600)%60};
    string HH_str{ZeroPaddedTime(HH)};
    string MM_str{ZeroPaddedTime(MM)};
    string SS_str{ZeroPaddedTime(SS)};
    string time{HH_str + ':' + MM_str + ':' + SS_str };
  return time; 
}

string Format::ZeroPaddedTime(long tt) {
  if ( tt<10 ) {
    return "0" + std::to_string(tt);
  }
  else {
    return std::to_string(tt);
  }
  

}