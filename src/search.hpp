#pragma once
#include "argument_parser.hpp"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void search(const fs::path& path, const std::string& txt, const SetFlags& userFlags);

void searchPaths(const SearchArguments& parsedSearchArguments, const SetFlags& userFlags);
