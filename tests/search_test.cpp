#include <gtest/gtest.h>

#include "search.hpp"

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

/*
 * Search File - Unit Testing
 */

// SearchMatch search(const fs::path& path, const std::string& txt, const SetFlags& userFlags);

TEST(SearchFileTest, ReadsFile)
{
    fs::path path {"../tests/data/basic_search.txt"};
    std::string searchPattern {"Hello"};
    SetFlags flags;
    SearchMatch searchMatch;

    searchMatch = search(path, searchPattern, flags);

    EXPECT_EQ(searchMatch.searchError, SearchMatch::SearchError::none);
}

TEST(SearchFileTest, BasicSearch)
{
    fs::path path {"../tests/data/basic_search.txt"};
    std::string searchPattern {"Hello"};
    SetFlags flags;
    SearchMatch searchMatch;

    searchMatch = search(path, searchPattern, flags);

    EXPECT_EQ(searchMatch.searchError, SearchMatch::SearchError::none);
    EXPECT_EQ(searchMatch.countOfLineMatches, 2);
}

TEST(SearchFileTest, CaseInsensitiveSearch)
{
    fs::path path {"../tests/data/basic_search.txt"};
    std::string searchPattern {"Hello"};
    SetFlags flags;
    flags.caseInsensitive = true;
    SearchMatch searchMatch;

    searchMatch = search(path, searchPattern, flags);

    EXPECT_EQ(searchMatch.searchError, SearchMatch::SearchError::none);
    EXPECT_EQ(searchMatch.countOfLineMatches, 4);
}

TEST(SearchFileTest, InvertedSearch)
{
    fs::path path {"../tests/data/basic_search.txt"};
    std::string searchPattern {"Hello"};
    SetFlags flags;
    flags.invertMatch = true;
    SearchMatch searchMatch;

    searchMatch = search(path, searchPattern, flags);

    EXPECT_EQ(searchMatch.searchError, SearchMatch::SearchError::none);
    EXPECT_EQ(searchMatch.countOfLineMatches, 6);
}
TEST(SearchFileTest, FileDoesNotOpen)
{
    fs::path path {"../tests/data/THIS_DOES_NOT_EXIST.txt"};
    std::string searchPattern {"Hello"};
    SetFlags flags;
    SearchMatch searchMatch;

    searchMatch = search(path, searchPattern, flags);

    EXPECT_EQ(searchMatch.searchError, SearchMatch::SearchError::fileNotOpen);
    EXPECT_EQ(searchMatch.countOfLineMatches, 0);
}

/*
 * File Path Traversal Tests
 */

// vector<PathInfo> searchPaths(const SearchArguments& parsedSearchArguments, const SetFlags& userFlags);

TEST(FileTraversalTest, FileDoesNotExistSearch)
{
    SearchArguments parsedSearchArguments;
    parsedSearchArguments.userPaths.push_back(fs::path{"../tests/THIS_DOES_NOT_EXIST.txt"});
    SetFlags userFlags;

    std::vector<PathInfo> allPathInfo;

    allPathInfo = searchPaths(parsedSearchArguments, userFlags);

    ASSERT_EQ(allPathInfo.size(), 1);
    EXPECT_EQ(allPathInfo[0].searchError, SearchMatch::SearchError::none);
    EXPECT_EQ(allPathInfo[0].pathTraversalError, PathInfo::PathTraversalError::nonSearchablePath);
}

TEST(FileTraversalTest, NoRecursiveFlagError)
{
    SearchArguments parsedSearchArguments;
    parsedSearchArguments.userPaths.push_back(fs::path{"../tests/data"});
    SetFlags userFlags;

    // Reinforcing for testing
    userFlags.recursiveSearch = false;

    std::vector<PathInfo> allPathInfo;

    allPathInfo = searchPaths(parsedSearchArguments, userFlags);

    ASSERT_FALSE(allPathInfo.empty());

    EXPECT_EQ(allPathInfo[0].pathTraversalError, PathInfo::PathTraversalError::noRecursiveFlagDirectory);
}

TEST(FileTraversalTest, SuccessfulDirectorySearch)
{
    SearchArguments parsedSearchArguments;
    parsedSearchArguments.userPaths.push_back(fs::path{"../tests/data"});
    SetFlags userFlags;

    userFlags.recursiveSearch = true;

    std::vector<PathInfo> allPathInfo;

    allPathInfo = searchPaths(parsedSearchArguments, userFlags);

    ASSERT_FALSE(allPathInfo.empty());

    for(const auto& pathInfo : allPathInfo)
    {
        EXPECT_EQ(pathInfo.pathTraversalError, PathInfo::PathTraversalError::none);
    }
}
