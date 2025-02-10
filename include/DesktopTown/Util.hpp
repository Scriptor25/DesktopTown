#pragma once

namespace DesktopTown
{
    template<typename T>
    struct Rect
    {
        constexpr bool Contains(const T x, const T y) const
        {
            return Left <= x && x <= Right && Bottom <= y && y <= Top;
        }

        constexpr bool Contains(const Rect &other) const
        {
            return Contains(other.Left, other.Bottom) && Contains(other.Right, other.Top);
        }

        constexpr T Width() const
        {
            return Right - Left;
        }

        constexpr T Height() const
        {
            return Top - Bottom;
        }

        constexpr T Area() const
        {
            return Width() * Height();
        }

        T Left, Bottom, Right, Top;
    };
}
