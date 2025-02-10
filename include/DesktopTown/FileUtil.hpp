#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace DesktopTown
{
    std::vector<char> ReadFile(const std::filesystem::path &path, std::ios::openmode mode = {});
    [[nodiscard]] bool WriteFile(
        const std::filesystem::path &path,
        const std::vector<char> &data,
        std::ios::openmode mode = {});

    std::string ReadFileText(const std::filesystem::path &path, std::ios::openmode mode = {});
}
