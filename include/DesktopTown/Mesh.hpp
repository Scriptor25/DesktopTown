#pragma once

#include <DesktopTown/GL/GLBuffer.hpp>
#include <DesktopTown/GL/GLVertexArray.hpp>

namespace DesktopTown
{
    struct Vertex
    {
        glm::vec2 Position;
        glm::vec2 Texture;
    };

    class Mesh
    {
    public:
        Mesh();

        void SetVertices(const std::vector<Vertex> &vertices) const;
        void SetIndices(const std::vector<GLuint> &indices);

        void Draw() const;

    private:
        GLVertexArray m_VertexArray;
        GLBuffer m_VertexBuffer;
        GLBuffer m_IndexBuffer;

        GLsizei m_Count{};
    };
}
