#include "argument_parser.hpp"
#include "search.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <iterator>
#include <filesystem>
#include <array>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::vector<std::string> commandArguments;
    SetFlags userFlags;
    SearchArguments parsedSearchArguments;
    
    // Convert command-line arguments to strings for easier processing. 
    for(int i = 0; i < argc; ++i)
    {
        commandArguments.push_back(argv[i]);
    }
    
    // Input validation
    if(argc <= 1)
    {
        std::cerr << "Error Invalid Syntax: Hint: swiftGrep [OPTIONS] PATTERN PATH...]" << std::endl;
        return 1;
    }

    // Set flags if provided
    userFlags = parseFlags(commandArguments);

    // Check for userFlag errors
    if(userFlags.flagError != SetFlags::FlagParseError::none)
    {
            std::cerr << "Error Invalid Flag: Options: -i, -v, -c, -l, -f, -r" << "\n";
            return 2;
    }

    // Parse remaining command Arguments
    parsedSearchArguments = parseSearchArguments(userFlags,commandArguments);


    // Check for userFlag errors
    if(parsedSearchArguments.searchParseError!= SearchArguments::SearchParseError::none)
    {
            std::cerr << "Error Invalid Syntax: Hint: swiftGrep [OPTIONS] PATTERN PATH...]" << "\n";
            return 2;
    }

    searchPaths(parsedSearchArguments, userFlags);


    return 0;
}
