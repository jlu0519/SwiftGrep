#pragma once
#include "argument_parser.hpp"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

std::string lower(const std::string& txt);

void search(const fs::path& path, const std::string& txt, const SetFlags& userFlags);
