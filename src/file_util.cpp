#include <fstream>
#include <DesktopTown/DesktopTown.hpp>

std::vector<char> DesktopTown::ReadFile(const std::string& filename, const std::ios::openmode mode)
{
    std::fstream stream(filename, mode | std::ios::in | std::ios::ate);
    if (!stream.is_open())
        return {};

    std::vector<char> buffer(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(buffer.data(), buffer.size());
    stream.close();

    return buffer;
}

void DesktopTown::WriteFile(const std::string& filename, const std::vector<char>& data, const std::ios::openmode mode)
{
    std::fstream stream(filename, mode | std::ios::out);
    if (!stream.is_open())
        return;

    stream.write(data.data(), data.size());
    stream.close();
}

std::string DesktopTown::ReadFileText(const std::string& filename, std::ios::openmode mode)
{
    const auto data = ReadFile(filename, mode);
    return std::string(data.begin(), data.end());
}
