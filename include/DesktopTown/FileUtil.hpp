#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace DesktopTown
{
    std::vector<char> ReadFile(const std::string& filename, std::ios::openmode mode = {});
    void WriteFile(const std::string& filename, const std::vector<char>& data, std::ios::openmode mode = {});

    std::string ReadFileText(const std::string& filename, std::ios::openmode mode = {});
}
