#pragma once

#include <vector>
#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLBuffer final : public GLObject
    {
        explicit GLBuffer(GLuint name);

    public:
        GLBuffer();
        ~GLBuffer() override;

        GLBuffer(const GLBuffer &) = delete;
        GLBuffer &operator=(const GLBuffer &) = delete;
        GLBuffer(GLBuffer &&other) noexcept;
        GLBuffer &operator=(GLBuffer &&other) noexcept;

        [[nodiscard]] void *Map(GLbitfield access) const;
        [[nodiscard]] void *MapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) const;
        [[nodiscard]] bool Unmap() const;

        [[nodiscard]] GLDefer Bind(GLenum target) const;
        void Copy(
            const GLBuffer &write_buffer,
            GLintptr read_offset,
            GLintptr write_offset,
            GLsizeiptr size) const;

        template<typename T>
        void Data(const GLsizeiptr count, const GLenum usage) const
        {
            glNamedBufferData(m_Name, count * sizeof(T), nullptr, usage);
        }

        template<typename T>
        void Data(const std::vector<T> &data, const GLenum usage) const
        {
            glNamedBufferData(m_Name, data.size() * sizeof(T), data.data(), usage);
        }

        template<typename T>
        void SubData(const GLintptr offset, const std::vector<T> &data) const
        {
            glNamedBufferSubData(m_Name, offset * sizeof(T), data.size() * sizeof(T), data.data());
        }

        template<typename T>
        void ClearData(const GLenum internalformat, const GLenum format, const GLenum type, const T &data) const
        {
            glClearNamedBufferData(m_Name, internalformat, format, type, &data);
        }

        template<typename T>
        void ClearSubData(
            const GLenum internalformat,
            const GLintptr offset,
            const GLsizeiptr size,
            const GLenum format,
            const GLenum type,
            const T &data) const
        {
            glClearNamedBufferSubData(
                m_Name,
                internalformat,
                offset * sizeof(T),
                size * sizeof(T),
                format,
                type,
                &data);
        }

        template<typename T>
        void Storage(const GLsizeiptr count, const GLbitfield flags) const
        {
            glNamedBufferStorage(m_Name, count * sizeof(T), nullptr, flags);
        }

        template<typename T>
        void Storage(const std::vector<T> &data, const GLbitfield flags) const
        {
            glNamedBufferStorage(m_Name, data.size() * sizeof(T), data.data(), flags);
        }

        static GLBuffer Create();
    };
}
