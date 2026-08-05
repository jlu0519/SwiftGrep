#include "argument_parser.hpp"
#include <iostream>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

SetFlags parseFlags(const std::vector<std::string>& commandArguments)
{
    // Parse consecutive flags at the beginning of the command.
    SetFlags userFlags {};

    int numberOfArguments = static_cast<int>(commandArguments.size());
    int lengthOfArgument {};

    for( int i = 1; i < numberOfArguments; ++i)
    { 
        if(!commandArguments[i].empty())
        {
            if(commandArguments[i] == "--")
            {
                userFlags.endOfOptions = true;
                ++userFlags.flagCommandArguments;
                return userFlags;
            }
            else if (commandArguments[i][0] == '-')
            {
                lengthOfArgument = commandArguments[i].size();
                
                if(lengthOfArgument <= 1)
                {
                    userFlags.flagError = SetFlags::FlagParseError::loneDash;

                    return userFlags;
                }

                for(int j = 1; j < lengthOfArgument; ++j)
                {
                    if(commandArguments[i][j] == 'i')
                    {
                        userFlags.caseInsensitive = true;
                    }
                    else if(commandArguments[i][j] == 'v')
                    {
                        userFlags.invertMatch = true;
                    }
                    else if(commandArguments[i][j] == 'c')
                    {
                        userFlags.countOnly = true;
                    }
                    else if(commandArguments[i][j] == 'l')
                    {
                        userFlags.lineNumbers = true;
                    }
                    else if(commandArguments[i][j] == 'f')
                    {
                        userFlags.showFile = true;
                    }
                    else if(commandArguments[i][j] == 'r')
                    {
                        userFlags.recursiveSearch = true;
                        userFlags.showFile= true;
                    }
                    else
                    {
                        userFlags.flagError = SetFlags::FlagParseError::unrecognizedOptionCharacter;

                        return userFlags;
                    }
                }

            }
            else
            {
                return userFlags;
            }

            ++userFlags.flagCommandArguments;
        }
        else
        {
            return userFlags;
        }
    }
    return userFlags;
}

SearchArguments parseSearchArguments(SetFlags userFlags, 
        const std::vector<std::string>& commandArguments)
{
    SearchArguments userSearchArguments;
    int searchTxtIndex {};
    int pathStartIndex {};
    int numberOfArguments = static_cast<int>(commandArguments.size());

    // Determine the positions of the search text and first path
    searchTxtIndex = userFlags.flagCommandArguments + 1;
    pathStartIndex = searchTxtIndex + 1;

    // Input validation  
    if(numberOfArguments <= pathStartIndex)
    {

        userSearchArguments.searchParseError = SearchArguments::SearchParseError::noPathProvided;

        return userSearchArguments;
    }

    // Search-text extraction
    userSearchArguments.searchTxt = commandArguments[searchTxtIndex];

    // Collect all paths provided after search text.
    for(int i = pathStartIndex; i < numberOfArguments; ++i)
    {
        userSearchArguments.userPaths.push_back(commandArguments[i]);    
    }

    return userSearchArguments;
}
