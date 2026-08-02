#include "search.hpp"
#include <iostream>
#include <fstream>
#include <regex>

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
