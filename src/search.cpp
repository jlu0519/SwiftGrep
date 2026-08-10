#include "search.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>

namespace fs = std::filesystem;

void printMatchingText(const SetFlags& userFlags,const fs::path& path, const SearchMatch& searchMatch)
{
    if(userFlags.lineNumbers && userFlags.showFile)
    {
        std::cout << path << ":" << searchMatch.lineNumber << ":" << searchMatch.line << "\n";
    }
    else if(userFlags.lineNumbers)
    {
        std::cout << searchMatch.lineNumber << ":" << searchMatch.line << "\n";
    }
    else if(userFlags.showFile)
    {
        std::cout << path << ":" << searchMatch.line << "\n";
    }
    else
    {
        std::cout << searchMatch.line << "\n";
    }

}

void printCountOfMatches(const fs::path& path, const SearchMatch& searchMatch)
{
    std::cout << path << ":" <<  searchMatch.countOfLineMatches << "\n";
}

SearchMatch search(const fs::path& path, const std::string& txt, const SetFlags& userFlags)
{
    SearchMatch searchMatch;
    searchMatch.lineNumber = {1};
    std::ifstream file(path);

    if(!file.is_open()) 
    {
        searchMatch.searchError = SearchMatch::SearchError::fileNotOpen;
        return searchMatch;
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
        while(std::getline(file, searchMatch.line))
        {
             
            // Determine whether the current line matches the search text
            searchMatch.acceptedLine = std::regex_search(searchMatch.line, pattern);

            // Reverse the match decision when invert mode is enabled.
            if(userFlags.invertMatch)
            {
                searchMatch.acceptedLine = !searchMatch.acceptedLine;
            }

            // Accepted lines are either counted or formatted for output depending on the selected command-line flags.
            if(searchMatch.acceptedLine)
            {
                ++searchMatch.countOfLineMatches;

                if(!userFlags.countOnly)
                {
                    printMatchingText(userFlags, path, searchMatch);
                }
            }

            ++searchMatch.lineNumber;
        }
    }
    catch(const std::regex_error& error)
    {
        searchMatch.searchError = SearchMatch::SearchError::invalidRegularExpression;
        return searchMatch;
    }
    
    // Display the total number of accepted lines for this file.
    if(userFlags.countOnly)
    {
        printCountOfMatches(path, searchMatch);
    }
    
    // For Search Error State checking
    return searchMatch;
}

std::vector<PathInfo> searchPaths(const SearchArguments& parsedSearchArguments, const SetFlags& userFlags)
{
    SearchMatch searchMatch;
    PathInfo pathInfo;
    std::vector<PathInfo> allPathInfo;

    // Search each user-supplied path independently
    for(const auto& path : parsedSearchArguments.userPaths)
    {
        pathInfo.path = path;
        pathInfo.searchError = SearchMatch::SearchError::none;
        pathInfo.pathTraversalError = PathInfo::PathTraversalError::none;

        // Enable recursive traversal when recursive search is requested
        if(userFlags.recursiveSearch)
        {
            if(fs::is_regular_file(path))
            {
                searchMatch = search(path, parsedSearchArguments.searchTxt, userFlags);
                pathInfo.searchError = searchMatch.searchError;
                allPathInfo.push_back(pathInfo);
            }
            // Recursively search every regular file beneath the directory.
            else if(fs::is_directory(path))
            {
                for(auto& directoryEntry : fs::recursive_directory_iterator(path,fs::directory_options::skip_permission_denied))
                {
                    fs::path childPath = directoryEntry.path();

                    if(fs::is_regular_file(childPath))
                    {
                        pathInfo.path = childPath;

                        searchMatch = search(childPath, parsedSearchArguments.searchTxt, userFlags);
                        pathInfo.searchError = searchMatch.searchError;
                        allPathInfo.push_back(pathInfo);
                    }
                }
            }
            // Report paths that are neither files nor directories
            else
            {
                pathInfo.pathTraversalError = PathInfo::PathTraversalError::nonSearchablePath;
                allPathInfo.push_back(pathInfo);
            }
        }
        else
        {
            // Search a single file without recursion
            if(fs::is_regular_file(path))
            {
                searchMatch = search(path, parsedSearchArguments.searchTxt, userFlags);
                pathInfo.searchError = searchMatch.searchError;
                allPathInfo.push_back(pathInfo);
            }
            else if(fs::is_directory(path))
            {
                pathInfo.pathTraversalError = PathInfo::PathTraversalError::noRecursiveFlagDirectory;
                allPathInfo.push_back(pathInfo);

                continue;
            }
            else
            {
                pathInfo.pathTraversalError = PathInfo::PathTraversalError::nonSearchablePath;
                allPathInfo.push_back(pathInfo);
            }
        }
    }
    return allPathInfo;
}
