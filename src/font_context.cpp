#include <DesktopTown/FontContext.hpp>

DesktopTown::FontContext::FontContext()
    : m_Atlas(GLTexture::Create())
{
    FT_Init_FreeType(&m_FT);
}

DesktopTown::FontContext::~FontContext()
{
    FT_Done_FreeType(m_FT);
}

void DesktopTown::FontContext::Init(GLProgram&& program, GLVertexArray&& vertex_array, GLBuffer&& vertex_buffer)
{
    m_Program = std::move(program);
    m_VertexArray = std::move(vertex_array);
    m_VertexBuffer = std::move(vertex_buffer);
}

constexpr auto MIN_CHAR = 0x020;
constexpr auto MAX_CHAR = 0x50C;
constexpr auto CHAR_RANGE = MAX_CHAR - MIN_CHAR + 1;
constexpr auto NUM_TILES_RT = static_cast<int>(ceil(sqrt(CHAR_RANGE)));
constexpr auto TILE_DELTA = 1.f / static_cast<float>(NUM_TILES_RT);

void DesktopTown::FontContext::LoadFont(
    const std::string& filename,
    const FT_UInt width,
    const FT_UInt height,
    const FT_Long index)
{
    FT_Face font;
    FT_New_Face(m_FT, filename.c_str(), index, &font);
    FT_Set_Pixel_Sizes(font, width, height);

    unsigned max_width = 0, max_height = 0;
    for (int c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(font, c, FT_LOAD_DEFAULT);

        const auto c_width = font->glyph->bitmap.width;
        const auto c_height = font->glyph->bitmap.rows;

        if (c_width > max_width) max_width = c_width;
        if (c_height > max_height) max_height = c_height;
    }

    for (int c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(font, c, FT_LOAD_DEFAULT);

        const auto c_width = font->glyph->bitmap.width;
        const auto c_height = font->glyph->bitmap.rows;

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
            {font->glyph->bitmap_left, font->glyph->bitmap_top},
            font->glyph->advance.x,
        };
    }

    m_Width = NUM_TILES_RT * max_width;
    m_Height = NUM_TILES_RT * max_height;

    m_Atlas.Bind(GL_TEXTURE_2D);
    m_Atlas.Image2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        static_cast<GLsizei>(m_Width),
        static_cast<GLsizei>(m_Height),
        GL_RED,
        GL_UNSIGNED_BYTE, nullptr);
    m_Atlas.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Atlas.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_Atlas.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_Atlas.SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(font, c, FT_LOAD_RENDER);

        const auto c_width = font->glyph->bitmap.width;
        const auto c_height = font->glyph->bitmap.rows;

        const auto off = c - MIN_CHAR;
        const auto i = off % NUM_TILES_RT * max_width;
        const auto j = off / NUM_TILES_RT * max_height;

        m_Atlas.SubImage2D(
            0,
            static_cast<GLint>(i),
            static_cast<GLint>(j),
            static_cast<GLsizei>(c_width),
            static_cast<GLsizei>(c_height),
            GL_RED,
            GL_UNSIGNED_BYTE,
            font->glyph->bitmap.buffer);
    }

    FT_Done_Face(font);
}

void DesktopTown::FontContext::DrawText(
    const std::wstring& text,
    const float start_x,
    const float start_y,
    const float scale,
    const glm::vec3& color)
{
    m_Program.Use();
    m_Program.SetUniform<Uniform_Float3>("COLOR", color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    m_Atlas.Bind(GL_TEXTURE_2D);
    m_VertexArray.Bind();
    m_VertexBuffer.Bind(GL_ARRAY_BUFFER);

    constexpr auto VERTEX_SIZE = 4 * sizeof(GLfloat);
    constexpr auto VERTICES_PER_CHAR = 6;

    const auto vertex_count = text.size() * VERTICES_PER_CHAR;
    m_VertexBuffer.Data<GLfloat>(vertex_count * VERTEX_SIZE, GL_DYNAMIC_DRAW);

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

        const std::vector vertices
        {
            xpos, ypos + h, texture_.x, texture_.y,
            xpos, ypos, texture_.x, texture_.w,
            xpos + w, ypos, texture_.z, texture_.w,

            xpos, ypos + h, texture_.x, texture_.y,
            xpos + w, ypos, texture_.z, texture_.w,
            xpos + w, ypos + h, texture_.z, texture_.y
        };

        m_VertexBuffer.SubData(i * VERTICES_PER_CHAR * VERTEX_SIZE, vertices);
        x += static_cast<float>(advance_ >> 6) * scale;
    }

    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}

void DesktopTown::FontContext::DrawAtlas(
    const float start_x,
    const float start_y,
    const float scale,
    const glm::vec3& color)
{
    m_Program.Use();
    m_Program.SetUniform<Uniform_Float3>("COLOR", color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    m_Atlas.Bind(GL_TEXTURE_2D);

    const auto fw = static_cast<float>(m_Width) * scale;
    const auto fh = static_cast<float>(m_Height) * scale;

    const std::vector vertices
    {
        start_x, start_y + fh, 0.f, 0.f,
        start_x, start_y, 0.f, 1.f,
        start_x + fw, start_y, 1.f, 1.f,

        start_x, start_y + fh, 0.f, 0.f,
        start_x + fw, start_y, 1.f, 1.f,
        start_x + fw, start_y + fh, 1.f, 0.f
    };

    m_VertexArray.Bind();
    m_VertexBuffer.Bind(GL_ARRAY_BUFFER);
    m_VertexBuffer.Data(vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
