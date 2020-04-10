/*
* driver.cpp
* Sean Murthy, Nick DeMarco
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Intialize tester and start unit tests
*/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <charconv>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <cstddef>

#include "tester.h"

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

void processCmdLine(std::vector<std::string_view> arguments);

void setDefaults();

int main(int argc, char* argv[])
{
    std::vector<std::string_view> arguments;
    for (int i = 0; i < argc; ++i)
        arguments.emplace_back(argv[i]);
    
    processCmdLine(arguments);

   return getTestsFailed();
}

static const std::string defaultHeaderText("Running $exe");

void processCmdLine(std::vector<std::string_view> arguments)
{
   if (arguments.size() == 0)
   {
      setDefaults();
      return;
   }

   bool printHeader{ true };
   bool printSummary{ true };
   std::ofstream fileOut;
   std::string headerText(defaultHeaderText);
   std::string passReportMode("auto");
   unsigned short failThreshold(0);
   std::string_view fileOpenMode;
   std::string outputFilename;

   // begin parsing cmd-line arguments
   for (std::size_t i = 1; i < arguments.size(); i += 2)
   {
      if (arguments[i] == "-h")
         printHeader = arguments[i + 1] != "no";
      else if (arguments[i] == "-ht")
         headerText = arguments[i + 1];
      else if (arguments[i] == "-p")
         passReportMode = arguments[i + 1];
      else if (arguments[i] == "-s")
         printSummary = arguments[i + 1] != "no";
      else if (arguments[i] == "-t")
      {
         std::string_view& v = arguments[i + 1];
         std::from_chars(v.data(), v.data() + v.size(), failThreshold);
         setFailThreshold(failThreshold);
      }
      else if (arguments[i] == "-f")
      {
         fileOpenMode = arguments[i];
         outputFilename = arguments[i + 1];
         setOutput(fileOut);
      }
      else if (arguments[i] == "-fo")
      {
         fileOpenMode = arguments[i];
         outputFilename = arguments[i + 1];
         setOutput(fileOut);
      }
      else if (arguments[i] == "-fa")
      {
         fileOpenMode = arguments[i];
         outputFilename = arguments[i + 1];
         setOutput(fileOut);
      }
   } //done parsing cmd-line arguments

   //extract the exe name without path or filename extension
   //the result is used to expand the macro $exe
   std::filesystem::path exePath(arguments[0]);
   std::string filenameNoExtension = exePath.replace_extension("").filename().string();

   //replace $exe macro in header text and file name
   if (printHeader && !headerText.empty())
   {
      std::string::size_type macroPosition = headerText.find("$exe");
      for (; macroPosition != std::string::npos; macroPosition = headerText.find("$exe"))
         headerText.replace(macroPosition, 4, filenameNoExtension);
   }
   else
      headerText = "";

   setHeaderText(headerText);

   if (!outputFilename.empty())
   {
      std::string::size_type macroPosition = outputFilename.find("$exe");
      for (; macroPosition != std::string::npos; macroPosition = outputFilename.find("$exe"))
         outputFilename.replace(macroPosition, 4, filenameNoExtension);
   }

   if (passReportMode == "detail")
       setPassReportMode(passReportMode::detail);
   else if (passReportMode == "indicate")
       setPassReportMode(passReportMode::indicate);
   else if (passReportMode == "none")
       setPassReportMode(passReportMode::none);
   else if (passReportMode == "auto")
   {
      if (fileOpenMode == "-f" || fileOpenMode == "-fo" || fileOpenMode == "-fa")
         setPassReportMode(passReportMode::detail);
      else
         setPassReportMode(passReportMode::indicate);
   }

   if (fileOpenMode == "-f")
   {
      
      std::filesystem::path outputFilePath = outputFilename;
      if (outputFilePath.extension() == "")
         outputFilePath.replace_extension(".out");
      if (!exists(outputFilePath))
      {
         fileOut.open(outputFilePath);
      }
      else
         std::cout << "File alredy exists. To overwrite use option -fo, to append use option -fa.";
   }
   else if (fileOpenMode == "-fo")
   {
      std::filesystem::path outputFilePath = outputFilename;
      if (outputFilePath.extension() == "")
         outputFilePath.replace_extension(".out");
      fileOut.open(outputFilePath);
   }
   else if (fileOpenMode == "-fa")
   {
      setOutput(fileOut);
      std::filesystem::path outputFilePath = outputFilename;
      if (outputFilePath.extension() == "")
         outputFilePath.replace_extension(".out");
      fileOut.open(outputFilePath, std::ios::app);
      fileOut << '\n';
   }

   try
   {
      runTests();
   }
   catch (const std::string & msg)
   {
      std::cout << msg << '\n'; //TODO: print in appropriate stream based in cmd-line
   }

   if (printSummary)
      summarizeTests();
}

void setDefaults()
{

}
