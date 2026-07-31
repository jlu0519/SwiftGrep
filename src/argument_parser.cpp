#include "argument_parser.hpp"
#include <iostream>
#include <cstdlib>

SetFlags parseFlags(const std::vector<std::string>& commandArguments)
{
    // Parse consecutive flags at the beginning of the command.
    SetFlags userFlags;
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
                    std::cerr << "Invalid Option: Hint: [-i] [-v] [-c] [-l] [-f] [-r]" << "\n";
                    std::exit(2);
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
                        std::cerr << "Invalid Option: Hint: [-i] [-v] [-c] [-l] [-f] [-r]" << "\n";
                        std::exit(2);
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
