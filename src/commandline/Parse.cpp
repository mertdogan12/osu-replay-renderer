#include "Parse.hpp"

namespace osuRenderer {
    Parse::Parse(char** begin, char** end): begin(begin), end(end) {}

    bool Parse::cmdOptionExists(const std::string &option)
    {
        return std::find(begin, end, option) != end;
    }

    char* Parse::getCmdOption(const std::string &option)
    {
        char** itr = std::find(begin, end, option);

        if (itr != end && ++itr != end)
        {
            return *itr;
        }

        return 0;
    }
}
