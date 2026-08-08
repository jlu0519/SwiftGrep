#include <gtest/gtest.h>

#include "argument_parser.hpp"

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

/*
 * Parse Flags Function - Unit Testing
*/

TEST(ParseFlagsTest, EnablesCaseInsensitiveFlag)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-i",
        "hello",
        "README.md"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_TRUE(result.caseInsensitive);
    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::none);
}

TEST(ParseFlagsTest, RecursiveFlagEnablesRecursiveSearchAndFileNames)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-r",
        "hello",
        "src"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_TRUE(result.recursiveSearch);
    EXPECT_TRUE(result.showFile);
    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::none);
}

TEST(ParseFlagsTest, ParseTwoCombinedFlags)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-il",
        "hello",
        "README.md"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_TRUE(result.caseInsensitive);
    EXPECT_TRUE(result.lineNumbers);
    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::none);
}

TEST(ParseFlagsTest, ParseThreeCombinedFlags)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-ilf",
        "hello",
        "README.md"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_TRUE(result.caseInsensitive);
    EXPECT_TRUE(result.lineNumbers);
    EXPECT_TRUE(result.showFile);
    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::none);
}

TEST(ParseFlagsTest, ParsingTwoSeperateFlags)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-i",
        "-f",
        "hello",
        "README.md"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_TRUE(result.caseInsensitive);
    EXPECT_TRUE(result.showFile);
    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::none);
}


TEST(ParseFlagsTest, ParsingThreeSeperateFlags)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-i",
        "-l",
        "-f",
        "hello",
        "README.md"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_TRUE(result.caseInsensitive);
    EXPECT_TRUE(result.lineNumbers);
    EXPECT_TRUE(result.showFile);
    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::none);
}

TEST(ParseFlagsTest, ParseEndOfOptions)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "--",
        "-l",
        "hello",
        "README.md"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_TRUE(result.endOfOptions);
    EXPECT_FALSE(result.lineNumbers);
    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::none);
}

TEST(ParseFlagsTest, LoneDashError)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::loneDash);
}

TEST(ParseFlagsTest, UnrecognizedOptionCharacterError)
{
    std::vector<std::string> commandArguments {
        "./swiftgrep",
        "-x",
        "hello",
        "README.md"
    };

    SetFlags result {
        parseFlags(commandArguments)
    };

    EXPECT_EQ(result.flagError, SetFlags::FlagParseError::unrecognizedOptionCharacter);
}

/*
 * Parse Search Argument Function Unit Testing
*/

TEST(ParseSearchArgumentsTest, ParsesSearchTextAndSinglePath)
{
    SetFlags flags;

    std::vector<std::string> commandArguments {
        "./swiftGrep",
        "hello",
        "README.md"
    };

    SearchArguments result {
        parseSearchArguments(flags, commandArguments)
    };

    // Bounds Check
    ASSERT_EQ(result.userPaths.size(), 1);

    EXPECT_EQ(result.searchTxt, "hello");
    EXPECT_EQ(result.userPaths[0], fs::path{"README.md"});
    EXPECT_EQ(result.searchParseError, SearchArguments::SearchParseError::none);

}

TEST(ParseSearchArgumentsTest, NoPathProvidedError)
{
    SetFlags flags;

    std::vector<std::string> commandArguments {
        "./swiftGrep",
        "hello",
    };

    SearchArguments result {
        parseSearchArguments(flags, commandArguments)
    };

    EXPECT_EQ(result.searchParseError, SearchArguments::SearchParseError::noPathProvided);
}

TEST(ParseSearchArgumentsTest, ParsesMultipleFilePaths)
{
    SetFlags flags;

    std::vector<std::string> commandArguments {
        "./swiftGrep",
        "hello",
        "src/search.cpp",
        "src/argument_parser.cpp"
    };

    SearchArguments result {
        parseSearchArguments(flags, commandArguments)
    };
    
    // Bounds Check
    ASSERT_EQ(result.userPaths.size(),2);

    EXPECT_EQ(result.userPaths[0], fs::path{"src/search.cpp"});
    EXPECT_EQ(result.userPaths[1], fs::path{"src/argument_parser.cpp"});
    EXPECT_EQ(result.searchParseError, SearchArguments::SearchParseError::none);

}

TEST(ParseSearchArgumentsTest, ParsesSearchArgumentsAfterOneFlag)
{
    SetFlags flags;
    flags.flagCommandArguments = 1;

    std::vector<std::string> commandArguments {
        "./swiftGrep",
        "-i",
        "hello",
        "src/main.cpp"
    };

    SearchArguments result {
        parseSearchArguments(flags, commandArguments)   
    };

    // Bounds check
    ASSERT_EQ(result.userPaths.size(),1);

    EXPECT_EQ(result.userPaths[0], fs::path{"src/main.cpp"});
    EXPECT_EQ(result.searchParseError, SearchArguments::SearchParseError::none);
}

TEST(ParseSearchArgumentsTest, ParsesSearchArgumentsAfterTwoFlags)
{
    SetFlags flags;
    flags.flagCommandArguments = 2;

    std::vector<std::string> commandArguments {
        "./swiftGrep",
        "-i",
        "-l",
        "hello",
        "src/main.cpp"
    };

    SearchArguments result {
        parseSearchArguments(flags, commandArguments)   
    };

    // Bounds check
    ASSERT_EQ(result.userPaths.size(),1);

    EXPECT_EQ(result.userPaths[0], fs::path{"src/main.cpp"});
    EXPECT_EQ(result.searchParseError, SearchArguments::SearchParseError::none);
}
