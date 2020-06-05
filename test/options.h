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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>
#include <string>

#include "tester.h"

struct Options {
   bool printHeader{ true };
   bool printSummary{ true };
   std::string headerText{ "Running $exe" };
   std::string_view passReportMode; // empty value treated as "auto" in getPassReportMode                    
   unsigned short failThreshold = 0;
   std::string_view fileOpenMode;
   std::string outputFilename;
   std::string filenameNoExt;
};


Options getOptions(char* arguments[], const std::size_t size);

void applyOptions(Options options);

passReportMode getPassReportMode(const std::string_view& value, bool fileOutput);

void replace_all(std::string& str, const std::string& substr, const std::string& new_substr);

#endif