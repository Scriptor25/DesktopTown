#pragma once

#include <vector>

namespace DesktopTown
{
    template<typename T>
    class VecView
    {
    public:
        VecView(std::vector<T> &source, const size_t beg)
            : m_Source(source),
              m_Beg(beg)
        {
        }

        template<size_t N>
        VecView &operator=(const std::array<T, N> &values)
        {
            std::copy(values.begin(), values.end(), m_Source.begin() + m_Beg);
            return *this;
        }

        VecView &operator=(const std::vector<T> &values)
        {
            std::copy(values.begin(), values.end(), m_Source.begin() + m_Beg);
            return *this;
        }

    private:
        std::vector<T> &m_Source;
        size_t m_Beg;
    };
}
