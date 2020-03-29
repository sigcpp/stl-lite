/*
* tester.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained. 
* - Attribution may be augmented to include additional authors 
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Declare testing infrastructure
*/

#include <iostream>

enum class passReportMode { none, indicate, detail };

void setPassReportMode(passReportMode mode);
void setFailThreshold(unsigned short value);
void setMaxFailThreshold();
void setOutput(std::ostream& o);

unsigned getTestsDone();
unsigned getTestsFailed();

void verify(bool success, const char* msg);
void summarizeTests();