/*
* tester.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice shown above must be retained. 
* - Attribution may be augmented to include additional authors. 
* - Copyright info cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Declare testing infrastructure
*/

enum class passReportMode { none, indicate, detail };

void setPassReportMode(passReportMode mode);
void setFailThreshold(unsigned value);
void setMaxFailThreshold();

void assert(bool success, const char* msg);
void summarizeTests();