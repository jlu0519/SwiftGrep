#include <string>

bool containsNeedle(const std::string& line)
{
    return line.find("needle") != std::string::npos;
}

// ERROR is uppercase intentionally.
// error is lowercase intentionally.
