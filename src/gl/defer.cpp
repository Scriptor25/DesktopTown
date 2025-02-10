#include <iostream>
#include <DesktopTown/GL.hpp>

DesktopTown::GLDefer::GLDefer(std::function<void()> &&defer)
    : m_Defer(std::move(defer))
{
}

DesktopTown::GLDefer::~GLDefer()
{
    if (m_Defer)
        m_Defer();
}

DesktopTown::GLDefer::GLDefer(GLDefer &&other) noexcept
    : m_Defer(std::move(other.m_Defer))
{
}

DesktopTown::GLDefer &DesktopTown::GLDefer::operator=(GLDefer &&other) noexcept
{
    std::swap(m_Defer, other.m_Defer);
    return *this;
}
