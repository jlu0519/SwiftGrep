#include "search.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>

namespace fs = std::filesystem;

void search(const fs::path& path, const std::string& txt, const SetFlags& userFlags)
{
    int lineNumber = 1;
    int countOfLineMatches{};
    std::string line;
    std::ifstream file(path);

    if(!file.is_open()) 
    {
        std::cerr << "Error opening file " << path << "!" << "\n";
        return;
    }
    
    std::regex_constants::syntax_option_type regexOptions = 
        std::regex_constants::ECMAScript;

    // Turn on case insensitive regex option
    if(userFlags.caseInsensitive)
    {
        regexOptions |= std::regex_constants::icase;
    }

    try
    {
        std::regex pattern{txt, regexOptions};
        
        // Process each line independently, determining whether it should be accepted based on active search flags.
        while(std::getline(file, line))
        {
             
            // Determine whether the current line matches the search text
            bool acceptedLine = std::regex_search(line, pattern);

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
    }
    catch(const std::regex_error& error)
    {
        std::cerr << "Invalid regular expression: " << error.what() << '\n';
        return;
    }
    
    // Display the total number of accepted lines for this file.
    if(userFlags.countOnly)
    {
        std::cout << path << ":" <<  countOfLineMatches << "\n";
    }
}

void searchPaths(const SearchArguments& parsedSearchArguments, const SetFlags& userFlags)
{
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
}
