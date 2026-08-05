#pragma once
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

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

    enum  class FlagParseError{none,unrecognizedOptionCharacter,loneDash};

    FlagParseError flagError {FlagParseError::none};
};

struct SearchArguments
{
    std::string searchTxt;
    std::vector<fs::path> userPaths; 

    enum  class SearchParseError{none,noPathProvided,noSearchPatternProvided};

    SearchParseError searchParseError {SearchParseError::none};
};

SetFlags parseFlags(const std::vector<std::string>& commandArguments);

SearchArguments parseSearchArguments(SetFlags userFlags, 
        const std::vector<std::string>& commandArguments);
