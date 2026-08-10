#pragma once
#include "argument_parser.hpp"
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

struct SearchMatch
{
    bool acceptedLine {false};
    int lineNumber {};
    std::string line;
    int countOfLineMatches {};

    enum class SearchError {none, fileNotOpen, invalidRegularExpression};

    SearchError searchError {SearchError::none};
};

struct PathInfo
{
    fs::path path;

    enum class PathTraversalError {none, noRecursiveFlagDirectory, nonSearchablePath};

    PathTraversalError pathTraversalError {PathTraversalError::none};
    SearchMatch::SearchError searchError {SearchMatch::SearchError::none};
};

void printCountOfMatches(const fs::path& path, const SearchMatch& searchMatch);

void printMatchingText(const SetFlags& userFlags,const fs::path& path, const SearchMatch& searchMatch);

SearchMatch search(const fs::path& path, const std::string& txt, const SetFlags& userFlags);

std::vector<PathInfo> searchPaths(const SearchArguments& parsedSearchArguments, const SetFlags& userFlags);
