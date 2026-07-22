#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

struct set_flags(std::vector<std::string> flags)
{
    struct setFlags
    {
        bool caseInsensitive {false};
        bool invertMatch {false};
        bool countOnly {false};
        bool lineNumbers {false};
        bool showFile {false};
    };

    for(const auto& flag : flags)
    {
        if(flag == "-i")
        {
            caseInsenstive = true;
        }
        if(flag == "-v")
        {
            invertMatch = true;
        }
        if(flag == "-c")
        {
            countOnly = true;
        }
        if(flag == "-l")
        {
            lineNumbers = true;
        }
        if(flag == "-f")
        {
            showFile = true;
        }
    }

    return setFlags;
}

// Converts string to lower case - Used in case-insensitive search.
std::string lower(const std::string& str)
{
    std::string lowercaseText;
    std::transform(str.begin(),str.end(),std::back_inserter(lowercaseText), tolower);         
    return lowercaseText;
}

void search(const std::string& fileName, std::ifstream& file, const std::string& txt, struct setFlags)
{
    int lineNumber = 1;
    int countOfLineMatches{};
    std::string line;
    std::string searchTxt = txt;

    if(caseInsensitive)
    {
        searchTxt = lower(searchTxt);
    }

    while(std::getline(file,line))
    {
        if(caseInsensitive)
        {
            std::string lowercaseLine = lower(line);

            if(lowercaseLine.find(searchTxt) != std::string::npos)
            {
                std::cout << fileName << ":" << lineNumber << ":" << line << "\n";
            }
        }
        else if(invertMatch)
        {
            if(line.find(searchTxt) == std::string::npos)
            {
                std::cout << line << "\n";
            }
        }
        else if(countOnly)
        {
            if(line.find(searchTxt) != std::string::npos)
            {
                countOfLineMatches++;
            }
        }
        else if(lineNumbers)
        {
            if(line.find(searchTxt) != std::string::npos)
            {
                std::cout << lineNumber << ":" << line << "\n";
            }
        }
        else if(showFile)
        {
            if(line.find(searchTxt) != std::string::npos)
            {
                std::cout << fileName << ":" << line << "\n";
            }
        }
        else
        {
            if(line.find(searchTxt) != std::string::npos)
            {
                std::cout << line << "\n";
            }
        }
                
        lineNumber++;
    }

    if(countOnly)
    {
        std::cout << "Line Matches: " << countOfLineMatches << "\n";
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::string> commandArguments;  
    std::vector<std::string> userFiles; 
    std::vector<std::string> flags;
    std::string searchTxt = "";

    int flagCount {};
    
    for(int i = 0; i < argc; ++i)
    {
        commandArguments.push_back(argv[i]);

        if(commandArguments[i].front() == "-")
        {
            flags.push_back(commandArgument[i]);
            flagCount++;
        }
    }

    if(argc <= 1)
    {
        std::cerr << "Error Invalid Syntax: Hint: swiftGrep [-flag] {searchtxt} {file1} [file2 ...]" << std::endl;
        return 1;
    }


    searchTxt = commandArguments[flagCount];

    // Setting flags 
    struct chosenflags = set_flags(flags);

    for(int i = ++flagCount; i < argc; ++i)
    {
        userFiles.push_back(commandArguments[i];    
    }

    for(const auto& fileName : userFiles)
    {
        // Open file using the stream constructor
        std::ifstream file(fileName);

        // Checking if file can be opened successfully 
        if(!file.is_open()) 
        {
            std::cerr << "Error opening file " << fileName << "!" <<  std::endl;
            continue;
        }
        
        // Search
        search(fileName, file, searchTxt, chosenFlags);

    }

    return 0;
}
