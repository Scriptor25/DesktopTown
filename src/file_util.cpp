#include <fstream>
#include <DesktopTown/FileUtil.hpp>

std::vector<char> DesktopTown::ReadFile(const std::string& filename, const std::ios::openmode mode)
{
    std::fstream stream(filename, mode | std::ios::in | std::ios::ate);
    if (!stream.is_open())
        return {};

    const auto count = stream.tellg();
    std::vector<char> buffer(count);
    stream.seekg(0, std::ios::beg);
    stream.read(buffer.data(), count);
    stream.close();

    return buffer;
}

void DesktopTown::WriteFile(const std::string& filename, const std::vector<char>& data, const std::ios::openmode mode)
{
    std::fstream stream(filename, mode | std::ios::out);
    if (!stream.is_open())
        return;

    stream.write(data.data(), static_cast<std::streamsize>(data.size()));
    stream.close();
}

std::string DesktopTown::ReadFileText(const std::string& filename, std::ios::openmode mode)
{
    const auto data = ReadFile(filename, mode);
    return {data.begin(), data.end()};
}
