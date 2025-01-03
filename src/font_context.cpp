#include <filesystem>
#include <DesktopTown/FontContext.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec2 Position;
    glm::vec2 Texture;
};

DesktopTown::FontContext::FontContext()
{
    FT_Init_FreeType(&m_FT);
}

DesktopTown::FontContext::~FontContext()
{
    if (m_FT)
        FT_Done_FreeType(m_FT);
}

constexpr auto MIN_CHAR = 0x020;
constexpr auto MAX_CHAR = 0x50C;
constexpr auto CHAR_RANGE = MAX_CHAR - MIN_CHAR + 1;
constexpr auto NUM_TILES_RT = static_cast<int>(ceil(sqrt(CHAR_RANGE)));
constexpr auto TILE_DELTA = 1.f / static_cast<float>(NUM_TILES_RT);

DesktopTown::FontContext::FontContext(FontContext&& other) noexcept
    : m_FT(other.m_FT),
      m_CharMap(std::move(other.m_CharMap)),
      m_Width(other.m_Width),
      m_Height(other.m_Height),
      m_Atlas(std::move(other.m_Atlas)),
      m_Program(std::move(other.m_Program)),
      m_VertexArray(std::move(other.m_VertexArray)),
      m_VertexBuffer(std::move(other.m_VertexBuffer)),
      m_IndexBuffer(std::move(other.m_IndexBuffer))
{
    other.m_FT = nullptr;
    other.m_CharMap.clear();
    other.m_Width = 0;
    other.m_Height = 0;
}

DesktopTown::FontContext& DesktopTown::FontContext::operator=(FontContext&& other) noexcept
{
    m_FT = other.m_FT;
    m_CharMap = std::move(other.m_CharMap);
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Atlas = std::move(other.m_Atlas);
    m_Program = std::move(other.m_Program);
    m_VertexArray = std::move(other.m_VertexArray);
    m_VertexBuffer = std::move(other.m_VertexBuffer);
    m_IndexBuffer = std::move(other.m_IndexBuffer);

    other.m_FT = nullptr;
    other.m_CharMap.clear();
    other.m_Width = 0;
    other.m_Height = 0;

    return *this;
}

void DesktopTown::FontContext::Init(const int window_width, const int window_height)
{
    m_Program = GLProgram::Create();

    auto raw_data = ReadFile("text.gl", std::ios::binary);
    if (raw_data.empty())
    {
        std::cerr << "Recompiling Program..." << std::endl;

        auto query = GLQuery::Create();
        query.Begin(GL_TIME_ELAPSED);

        {
            const auto shader = GLShader::Create(GL_VERTEX_SHADER);
            const auto source = ReadFileText("shader/text/vertex.glsl");
            shader.SetSource(source);
            if (shader.CompileAndCheck())
                m_Program.Attach(shader);
        }
        {
            const auto shader = GLShader::Create(GL_FRAGMENT_SHADER);
            const auto source = ReadFileText("shader/text/fragment.glsl");
            shader.SetSource(source);
            if (shader.CompileAndCheck())
                m_Program.Attach(shader);
        }

        (void)m_Program.LinkAndCheck();
        (void)m_Program.ValidateAndCheck();
        m_Program.DetachAll();

        query.End();
        GLuint64 time;
        glGetQueryObjectui64v(query.GetName(), GL_QUERY_RESULT, &time);
        std::cerr << "Done [" << time << " ns]" << std::endl;

        const auto [format_, data_] = m_Program.GetBinary();

        raw_data.resize(4 + data_.size());
        *reinterpret_cast<GLenum*>(raw_data.data()) = format_;
        for (unsigned i = 0; i < data_.size(); ++i)
            raw_data[i + 4] = data_[i];

        WriteFile("text.gl", raw_data, std::ios::binary);
    }
    else
    {
        std::cerr << "Loading Program Binary..." << std::endl;

        auto query = GLQuery::Create();
        query.Begin(GL_TIME_ELAPSED);

        const auto format = *reinterpret_cast<GLenum*>(raw_data.data());
        std::vector data(raw_data.begin() + 4, raw_data.end());
        const GLBinary binary
        {
            format,
            std::move(data),
        };
        (void)m_Program.SetBinaryAndCheck(binary);

        query.End();
        GLuint64 time;
        glGetQueryObjectui64v(query.GetName(), GL_QUERY_RESULT, &time);
        std::cerr << "Done [" << time << " ns]" << std::endl;
    }

    const auto fw = static_cast<float>(window_width);
    const auto fh = static_cast<float>(window_height);
    const auto projection = glm::ortho(0.f, fw, 0.f, fh);
    m_Program.SetUniformMatrix<Uniform_Matrix4x4>("PROJECTION", 1, GL_FALSE, &projection[0][0]);

    m_VertexArray = GLVertexArray::Create();

    m_VertexBuffer = GLBuffer::Create();
    m_VertexArray.VertexBuffer(0, m_VertexBuffer, 0, sizeof(Vertex));

    m_VertexArray.EnableAttrib(0);
    m_VertexArray.AttribFormat(0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    m_VertexArray.AttribBinding(0, 0);
    m_VertexArray.EnableAttrib(1);
    m_VertexArray.AttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Texture));
    m_VertexArray.AttribBinding(1, 0);

    m_IndexBuffer = GLBuffer::Create();
}

void DesktopTown::FontContext::LoadFont(
    const std::string& filename,
    const FT_UInt width,
    const FT_UInt height,
    const FT_Long index)
{
    FT_Face face;
    FT_New_Face(m_FT, filename.c_str(), index, &face);
    FT_Set_Pixel_Sizes(face, width, height);

    unsigned max_width = 0, max_height = 0;
    for (int c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(face, c, FT_LOAD_DEFAULT);

        const auto c_width = face->glyph->bitmap.width;
        const auto c_height = face->glyph->bitmap.rows;

        if (c_width > max_width) max_width = c_width;
        if (c_height > max_height) max_height = c_height;
    }

    m_CharMap.clear();

    for (int c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(face, c, FT_LOAD_DEFAULT);

        const auto c_width = face->glyph->bitmap.width;
        const auto c_height = face->glyph->bitmap.rows;

        const auto off = c - MIN_CHAR;
        const auto i = off % NUM_TILES_RT;
        const auto j = off / NUM_TILES_RT;

        const auto tx = static_cast<float>(i) * TILE_DELTA;
        const auto ty = static_cast<float>(j) * TILE_DELTA;

        const auto dx = TILE_DELTA * (static_cast<float>(c_width) / static_cast<float>(max_width));
        const auto dy = TILE_DELTA * (static_cast<float>(c_height) / static_cast<float>(max_height));

        m_CharMap[c] = {
            {tx, ty, tx + dx, ty + dy},
            {c_width, c_height},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            face->glyph->advance.x,
        };
    }

    m_Width = NUM_TILES_RT * max_width;
    m_Height = NUM_TILES_RT * max_height;

    m_Atlas = GLTexture::Create(GL_TEXTURE_2D);
    m_Atlas.Storage(1, GL_R32F, m_Width, m_Height);
    m_Atlas.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Atlas.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_Atlas.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_Atlas.SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        const GLsizei c_width = face->glyph->bitmap.width;
        const GLsizei c_height = face->glyph->bitmap.rows;

        const auto off = c - MIN_CHAR;
        const GLint i = off % NUM_TILES_RT * max_width;
        const GLint j = off / NUM_TILES_RT * max_height;

        m_Atlas.SubImage(0, i, j, c_width, c_height, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    }

    FT_Done_Face(face);
}

void DesktopTown::FontContext::DrawText(
    const std::wstring& text,
    const float start_x,
    const float start_y,
    const float scale,
    const glm::vec3& color)
{
    m_VertexBuffer.Data<Vertex>(text.size() * 4, GL_DYNAMIC_DRAW);
    m_IndexBuffer.Data<GLuint>(text.size() * 6, GL_DYNAMIC_DRAW);

    auto x = start_x;
    for (unsigned i = 0; i < text.size(); ++i)
    {
        auto& c = text[i];
        auto& [
            texture_,
            size_,
            bearing_,
            advance_
        ] = m_CharMap[c];

        const auto xpos = x + static_cast<float>(bearing_.x) * scale;
        const auto ypos = start_y - static_cast<float>(size_.y - bearing_.y) * scale;

        const auto w = static_cast<float>(size_.x) * scale;
        const auto h = static_cast<float>(size_.y) * scale;

        const std::vector<Vertex> vertices
        {
            {{xpos + 0, ypos + 0}, {texture_.x, texture_.w}},
            {{xpos + 0, ypos + h}, {texture_.x, texture_.y}},
            {{xpos + w, ypos + h}, {texture_.z, texture_.y}},
            {{xpos + w, ypos + 0}, {texture_.z, texture_.w}},
        };

        const GLuint base = i * vertices.size();
        const std::vector indices
        {
            base + 0u, base + 1u, base + 2u,
            base + 2u, base + 3u, base + 0u,
        };

        m_VertexBuffer.SubData(i * vertices.size(), vertices);
        m_IndexBuffer.SubData(i * indices.size(), indices);

        x += static_cast<float>(advance_ >> 6) * scale;
    }

    m_VertexArray.Bind();
    m_IndexBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
    m_Atlas.BindUnit(0);
    m_Program.Use();
    m_Program.SetUniform<Uniform_Float3>("COLOR", color.x, color.y, color.z);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawElements(GL_TRIANGLES, text.size() * 6, GL_UNSIGNED_INT, nullptr);

    glDisable(GL_BLEND);
}

void DesktopTown::FontContext::DrawAtlas(
    const float start_x,
    const float start_y,
    const float scale,
    const glm::vec3& color)
{
    const auto fw = static_cast<float>(m_Width) * scale;
    const auto fh = static_cast<float>(m_Height) * scale;

    const std::vector<Vertex> vertices
    {
        {{start_x + 00, start_y + 00}, {0.f, 1.f}},
        {{start_x + 00, start_y + fh}, {0.f, 0.f}},
        {{start_x + fw, start_y + fh}, {1.f, 0.f}},
        {{start_x + fw, start_y + 00}, {1.f, 1.f}},
    };

    const std::vector indices
    {
        0u, 1u, 2u,
        2u, 3u, 0u,
    };

    m_VertexBuffer.Data(vertices, GL_DYNAMIC_DRAW);
    m_IndexBuffer.Data(indices, GL_DYNAMIC_DRAW);

    m_VertexArray.Bind();
    m_IndexBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
    m_Atlas.BindUnit(0);
    m_Program.Use();
    m_Program.SetUniform<Uniform_Float3>("COLOR", color.x, color.y, color.z);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glDisable(GL_BLEND);
}
