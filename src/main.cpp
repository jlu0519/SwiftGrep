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
    std::vector<fs::path> userPaths; 
    std::string searchTxt;
    SetFlags userFlags;
    int searchTxtIndex {};
    int pathStartIndex {};
    
    // Convert command-line arguments to strings for easier processing. 
    for(int i = 0; i < argc; ++i)
    {
        commandArguments.push_back(argv[i]);
    }
    
    // Input validation
    if(argc <= 1)
    {
        std::cerr << "Error Invalid Syntax: Hint: swiftGrep [OPTIONS] SEARCH_TEXT PATH...]" << std::endl;
        return 1;
    }

    // Set flags if provided
    userFlags = parseFlags(commandArguments);

    // Determine the positions of the search text and first path
    searchTxtIndex = userFlags.flagCommandArguments + 1;
    pathStartIndex = searchTxtIndex + 1;

    // Input validation  
    if(argc <= pathStartIndex)
    {
        std::cerr << "Error Invalid Syntax: Hint: swiftGrep [OPTIONS] SEARCH_TEXT PATH...]" << "\n";
        return 1;
    }

    // Search-text extraction
    searchTxt = commandArguments[searchTxtIndex];

    // Input validation
    if(searchTxt.length() == 2)  
    {
        if(searchTxt[0] == '-' && !userFlags.endOfOptions)
        {
            std::cerr << "Error Invalid Flag: Options: -i, -v, -c, -l, -f, -r" << "\n";
            return 2;
        }
    }

    // Collect all paths provided after search text.
    for(int i = pathStartIndex; i < argc; ++i)
    {
        userPaths.push_back(commandArguments[i]);    
    }

    // Search each user-supplied path independently
    for(const auto& path : userPaths)
    {
        // Enable recursive traversal when recursive search is requested
        if(userFlags.recursiveSearch)
        {
            if(fs::is_regular_file(path))
            {
                search(path, searchTxt, userFlags);
            }
            // Recursively search every regular file beneath the directory.
            else if(fs::is_directory(path))
            {
                for(auto& directoryEntry : fs::recursive_directory_iterator(path,fs::directory_options::skip_permission_denied))
                {
                    fs::path childPath = directoryEntry.path();

                    if(fs::is_regular_file(childPath))
                    {
                        search(childPath, searchTxt, userFlags);
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
                search(path, searchTxt, userFlags);
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
