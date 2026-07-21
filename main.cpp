#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Converts string to lower case - Used in case insenstive search.
std::string lower(const std::string& str)
{
    std::string lowercaseText;
    std::transform(str.begin(),str.end(),std::back_inserter(lowercaseText), tolower);         
    return lowercaseText;
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
    
    std::string line;
    int lineNumber = 1;

    // Convert the search text oncd for case-insenstive searches
    if(flag == "-i")
    {
    searchTxt = lower(searchTxt);
    }
    

    // Condition checks if a line was successfully read.
    // Only saving one line at a time to handle large files the same as smaller files.

    while(std::getline(file,line))
    {
        if(flag == "-i")
        {
            std::string lowercaseLine = lower(line);

            if(lowercaseLine.find(searchTxt) != std::string::npos)
            {
                std::cout << userFile << ":" << lineNumber << ":" << line << "\n";
            }
        }
        else
        {
            if(line.find(searchTxt) != std::string::npos)
            {
                std::cout << userFile << ":" << lineNumber << ":" << line << "\n";\
            }
        }

        lineNumber++;

    }

    return 0;
}
