#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

// Converts string to lower case - Used in case-insensitive search.
std::string lower(const std::string& str)
{
    std::string lowercaseText;
    std::transform(str.begin(),str.end(),std::back_inserter(lowercaseText), tolower);         
    return lowercaseText;
}

void search(const std::string& fileName, std::ifstream& file, const std::string& txt, const bool flag)
{
    int lineNumber = 1;
    std::string line;
    std::string searchTxt = txt;

    if(flag)
    {
        searchTxt = lower(searchTxt);
    }

    while(std::getline(file,line))
    {
        if(flag)
        {
            std::string lowercaseLine = lower(line);

            if(lowercaseLine.find(searchTxt) != std::string::npos)
            {
                std::cout << fileName << ":" << lineNumber << ":" << line << "\n";
            }
        }
        else
        {
            if(line.find(searchTxt) != std::string::npos)
            {
                std::cout << fileName << ":" << lineNumber << ":" << line << "\n";
            }
        }
        lineNumber++;
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::string> commandArguments;  
    std::string userFile = ""; 
    std::string searchTxt = "";
    bool caseInsensitive {false};

    for(int i = 0; i < argc; ++i)
    {
       commandArguments.push_back(argv[i]);
    }

    // Verify that both the file path and search pattern were provided
    // Store user-provided file path and search pattern
    //
        if(commandArguments[1] == "-i" && argc >= 4)
        {
            caseInsensitive = true;
            searchTxt = commandArguments[2];
            userFile = commandArguments[3];
        }
        else if(commandArguments[1] == "-i" && argc < 3)
        {
            std::cerr << "Error Invalid Syntax: Hint: swiftGrep [-flag] {searchtxt} {file.txt}" << std::endl;
            return 1;
        }
        else if(argc <= 2)
        {
            std::cerr << "Error Invalid Syntax: Hint: swiftGrep [-flag] {searchtxt} {file.txt}" << std::endl;
            return 2;
        }
        else
        {
            searchTxt = commandArguments[1];
            userFile = commandArguments[2];
        } 

    // Open file using the stream constructor
    std::ifstream file(userFile);

    // Checking if file can be opened successfully 
    if(!file.is_open()) 
    {
        std::cerr << "Error opening file!" << std::endl;
        return 3;
    }
   
    search(userFile, file, searchTxt, caseInsensitive);

    return 0;
}

