#include <DesktopTown/Mesh.hpp>

DesktopTown::Mesh::Mesh()
    : m_VertexArray(GLVertexArray::Create()),
      m_VertexBuffer(GLBuffer::Create()),
      m_IndexBuffer(GLBuffer::Create())
{
    m_VertexArray.VertexBuffer(0, m_VertexBuffer, 0, sizeof(Vertex));
    m_VertexArray.EnableAttrib(0);
    m_VertexArray.AttribFormat(0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    m_VertexArray.AttribBinding(0, 0);
    m_VertexArray.EnableAttrib(1);
    m_VertexArray.AttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Texture));
    m_VertexArray.AttribBinding(1, 0);
}

void DesktopTown::Mesh::SetVertices(const std::vector<Vertex>& vertices) const
{
    m_VertexBuffer.Data(vertices, GL_DYNAMIC_DRAW);
}

void DesktopTown::Mesh::SetIndices(const std::vector<GLuint>& indices)
{
    m_IndexBuffer.Data(indices, GL_DYNAMIC_DRAW);
    m_Count = indices.size();
}

void DesktopTown::Mesh::Draw() const
{
    const auto defer_va = m_VertexArray.Bind();
    const auto defer_ib = m_IndexBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);

    glDrawElements(GL_TRIANGLES, m_Count, GL_UNSIGNED_INT, nullptr);
}
