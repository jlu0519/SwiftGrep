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

    // Search each user-supplied path independently
    for(const auto& path : parsedSearchArguments.userPaths)
    {
        // Enable recursive traversal when recursive search is requested
        if(userFlags.recursiveSearch)
        {
            if(fs::is_regular_file(path))
            {
                search(path, parsedSearchArguments.searchTxt, userFlags);
            }
            // Recursively search every regular file beneath the directory.
            else if(fs::is_directory(path))
            {
                for(auto& directoryEntry : fs::recursive_directory_iterator(path,fs::directory_options::skip_permission_denied))
                {
                    fs::path childPath = directoryEntry.path();

                    if(fs::is_regular_file(childPath))
                    {
                        search(childPath, parsedSearchArguments.searchTxt, userFlags);
                    }
                }
            }
            // Report paths that are neither files nor directories
            else
            {
                std::cerr << path << ": not a searchable file or directory" << "\n";
            }
        }
        else
        {
            // Search a single file without recursion
            if(fs::is_regular_file(path))
            {
                search(path, parsedSearchArguments.searchTxt, userFlags);
            }
            else if(fs::is_directory(path))
            {
                std::cerr << path <<": is a directory. Enter flag -r to search directories." << "\n";
                continue;
            }
            else
            {
                std::cerr << path << ": not a searchable file or directory" << "\n";
            }
        }
    }

    return 0;
}
