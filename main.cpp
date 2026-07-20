#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char* argv[])
{
    // Store user-provided file path and search pattern
    std::string userFile = argv[1]; 
    std::string searchTxt = argv[2];

    // Open file using the stream constructor
    std::ifstream file(userFile);

    // Checking if file can be opened successfully 
    if(!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 1;

    // Condition checks if a line was successfully read.
    // Only saving one line at a time to handle large files the same as smaller files.
    while(std::getline(file,line)) {
        // Outputs line if Searched text is found
        if(line.find(searchTxt) != std::string::npos)
        {
            std::cout <<userFile << ":" << lineNumber << ":" << line << "\n";
        }

        lineNumber++;
    }

    file.close();
    return 0;
}

    


    
