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
    SearchMatch searchMatch;
    std::vector<PathInfo> allPathInfo;
    
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


    // Check for Search Argument errors
    if(parsedSearchArguments.searchParseError!= SearchArguments::SearchParseError::none)
    {
            std::cerr << "Error Invalid Syntax: Hint: swiftGrep [OPTIONS] PATTERN PATH...]" << "\n";
            return 2;
    }
    
    allPathInfo = searchPaths(parsedSearchArguments, userFlags);

    for(const auto& pathInfo : allPathInfo)
    {
        if(pathInfo.pathTraversalError ==
            PathInfo::PathTraversalError::noRecursiveFlagDirectory)
        {
            std::cerr << pathInfo.path << ": is a directory. Enter flag -r to search directories.\n";
        }
        else if(pathInfo.pathTraversalError ==
                PathInfo::PathTraversalError::nonSearchablePath)
        {
            std::cerr << pathInfo.path << ": not a searchable file or directory\n";
        }

        if(pathInfo.searchError ==
            SearchMatch::SearchError::fileNotOpen)
        {
            std::cerr << pathInfo.path << ": unable to open file\n";
        }
        else if(pathInfo.searchError ==
                SearchMatch::SearchError::invalidRegularExpression)
        {
            std::cerr << "Invalid regular expression\n";
        }
    }

    return 0;
}
