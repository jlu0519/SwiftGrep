#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <iterator>
#include <filesystem>
#include <array>
#include "argument_parser.hpp"

namespace fs = std::filesystem;

// Converts string to lower case - Used in case-insensitive search.
std::string lower(const std::string& txt)
{
    std::string lowercaseText;
    std::transform(
        txt.begin(),
        txt.end(),
        std::back_inserter(lowercaseText),
        [](char character)
        {
            return static_cast<char>(
                std::tolower(static_cast<unsigned char>(character))
            );
        }
    );

    return lowercaseText;
}

void search(const fs::path& path, const std::string& txt, const SetFlags& userFlags )
{
    int lineNumber = 1;
    int countOfLineMatches{};
    std::string line;
    std::string searchTxt = txt;
    std::ifstream file(path);

    if(!file.is_open()) 
    {
        std::cerr << "Error opening file " << path << "!" << "\n";
        return;
    }

    // Normalize the search text once to avoid repeated conversions during case insensitive search
    if(userFlags.caseInsensitive)
    {
        searchTxt = lower(txt);
    }
    
    // Process each line independently, determining whether it should be accepted based on active search flags.
    while(std::getline(file,line))
    {
        bool acceptedLine = false;

        // Determine whether the current line matches the search text
        if(userFlags.caseInsensitive)
        {
            std::string lowercaseLine = lower(line);

            if(lowercaseLine.find(searchTxt) != std::string::npos)
            {
                acceptedLine = true;
            }
        }
        else
        {
            if(line.find(searchTxt) != std::string::npos)
            {
                acceptedLine = true;
            }
        }
        
        // Reverse the match decision when invert mode is enabled.
        if(userFlags.invertMatch)
        {
            acceptedLine = !acceptedLine;
        }

        // Accepted lines are either counted or formatted for output depending on the selected command-line flags.
        if(acceptedLine)
        {
            if(userFlags.countOnly)
            {
                ++countOfLineMatches;
            }
            else
            {
                if(userFlags.lineNumbers && userFlags.showFile)
                {
                    std::cout << path << ":" << lineNumber << ":" << line << "\n";
                }
                else if(userFlags.lineNumbers)
                {
                    std::cout << lineNumber << ":" << line << "\n";
                }
                else if(userFlags.showFile)
                {
                    std::cout << path << ":" << line << "\n";
                }
                else
                {
                    std::cout << line << "\n";
                }
            }
        }

        ++lineNumber;
    }
    
    // Display the total number of accepted lines for this file.
    if(userFlags.countOnly)
    {
        std::cout << path << ":" <<  countOfLineMatches << "\n";
    }
}

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
