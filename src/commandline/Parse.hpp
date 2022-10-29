#pragma once

#include "algorithm"
#include "string"

namespace osuRenderer {
    class Parse
    {
    public:
        // Parses the arguments from the commandline
        Parse(char** begin, char** end);

        // checks if and command line option exists
        bool cmdOptionExists(const std::string& option);

        // Returns the corresponding value of the given command line option
        char* getCmdOption(const std::string& option);
        
    private:
        char** begin, ** end;
    };
}
