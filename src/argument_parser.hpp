#pragma once
#include <vector>
#include <string>

struct SetFlags
{
    bool caseInsensitive {false};    // -i
    bool invertMatch {false};        // -v
    bool countOnly {false};          // -c 
    bool lineNumbers {false};        // -l
    bool showFile {false};           // -f
    bool recursiveSearch {false};    // -r
    bool endOfOptions {false};       // --                               
    int flagCommandArguments {};
};

SetFlags parseFlags(const std::vector<std::string>& commandArguments);
