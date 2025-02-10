#include <fstream>
#include <DesktopTown/FileUtil.hpp>

std::vector<char> DesktopTown::ReadFile(const std::filesystem::path &path, const std::ios::openmode mode)
{
    std::fstream stream(path, mode | std::ios::in | std::ios::ate);
    if (!stream.is_open())
        return {};

    const auto count = stream.tellg();
    std::vector<char> buffer(count);
    stream.seekg(0, std::ios::beg);
    stream.read(buffer.data(), count);
    stream.close();

    return buffer;
}

bool DesktopTown::WriteFile(
    const std::filesystem::path &path,
    const std::vector<char> &data,
    const std::ios::openmode mode)
{
    std::fstream stream(path, mode | std::ios::out);
    if (!stream.is_open())
        return true;

    stream.write(data.data(), static_cast<std::streamsize>(data.size()));
    stream.close();

    return false;
}

std::string DesktopTown::ReadFileText(const std::filesystem::path &path, const std::ios::openmode mode)
{
    const auto data = ReadFile(path, mode);
    return {data.begin(), data.end()};
}
