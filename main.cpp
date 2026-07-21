#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Converts string to lower case - Used in case-insensitive search.
std::string lower(const std::string& str)
{
    std::string lowercaseText;
    std::transform(str.begin(),str.end(),std::back_inserter(lowercaseText), tolower);         
    return lowercaseText;
}

void search(const std::string& fileName, std::ifstream& file, const std::string& txt, const std::string& flag)
{
    int lineNumber = 1;
    std::string line;
    std::string searchTxt = txt;

    if(flag == "-i")
    {
        searchTxt = lower(searchTxt);
    }

    while(std::getline(file,line))
    {
        if(flag == "-i")
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
    // Verify that both the file path and search pattern were provided
    if(argc <= 2)
    {
        std::cerr << "Too few arguments provided!" << std::endl;
        return 1;
    }

    // Store user-provided file path and search pattern
    std::string userFile = argv[1]; 
    std::string searchTxt = argv[2];
    std::string flag = "";
    if(argc >= 4)
    {
        flag = argv[3];
    }

    // Open file using the stream constructor
    std::ifstream file(userFile);

    // Checking if file can be opened successfully 
    if(!file.is_open()) 
    {
        std::cerr << "Error opening file!" << std::endl;
        return 2;
    }
   
    search(userFile, file, searchTxt, flag);

    return 0;
}

