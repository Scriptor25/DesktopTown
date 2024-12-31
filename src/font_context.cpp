#include <DesktopTown/FontContext.hpp>

DesktopTown::FontContext::FontContext()
{
    FT_Init_FreeType(&m_FT);
}

DesktopTown::FontContext::~FontContext()
{
    FT_Done_FreeType(m_FT);
}

constexpr auto NUM_TILES_RT = 0x10;
constexpr auto TILE_DELTA = 1.f / static_cast<float>(NUM_TILES_RT);
constexpr auto MIN_CHAR = 0x00;
constexpr auto MAX_CHAR = 0xff;

void DesktopTown::FontContext::LoadFont(
    const std::string& filename,
    const FT_UInt width,
    const FT_UInt height,
    const FT_Long index)
{
    FT_Face font;
    FT_New_Face(m_FT, filename.c_str(), index, &font);
    FT_Set_Pixel_Sizes(font, width, height);

    int max_width = 0, max_height = 0;
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

        const auto i = c % NUM_TILES_RT;
        const auto j = c / NUM_TILES_RT;

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

    const auto atlas_width = NUM_TILES_RT * max_width;
    const auto atlas_height = NUM_TILES_RT * max_height;

    m_Atlas.Bind(GL_TEXTURE_2D);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        atlas_width,
        atlas_height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(font, c, FT_LOAD_RENDER);

        const auto c_width = font->glyph->bitmap.width;
        const auto c_height = font->glyph->bitmap.rows;

        const auto i = (c % NUM_TILES_RT) * max_width;
        const auto j = (c / NUM_TILES_RT) * max_height;

        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            i,
            j,
            c_width,
            c_height,
            GL_RED,
            GL_UNSIGNED_BYTE,
            font->glyph->bitmap.buffer);
    }

    FT_Done_Face(font);
}

void DesktopTown::FontContext::DrawText(
    const GLProgram& program,
    const GLVertexArrays& vertex_array,
    const GLBuffers& vertex_buffer,
    const std::wstring& text,
    const float start_x,
    const float start_y,
    const float scale,
    const glm::vec3& color)
{
    program.Use();
    program.SetUniform<UNIFORM_FLOAT_3>("COLOR", color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    m_Atlas.Bind(GL_TEXTURE_2D);

    vertex_array.Bind();

    auto x = start_x;
    for (const auto& c : text)
    {
        const auto& [
            texture_,
            size_,
            bearing_,
            advance_
        ] = m_CharMap[c];

        const auto xpos = x + bearing_.x * scale;
        const auto ypos = start_y - (size_.y - bearing_.y) * scale;

        const auto w = size_.x * scale;
        const auto h = size_.y * scale;

        const float vertices[6 * 4] = {
            xpos, ypos + h, texture_.x, texture_.y,
            xpos, ypos, texture_.x, texture_.w,
            xpos + w, ypos, texture_.z, texture_.w,

            xpos, ypos + h, texture_.x, texture_.y,
            xpos + w, ypos, texture_.z, texture_.w,
            xpos + w, ypos + h, texture_.z, texture_.y
        };

        vertex_buffer.Bind(GL_ARRAY_BUFFER);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (advance_ >> 6) * scale;
    }
}

void DesktopTown::FontContext::DrawAtlas(
    const GLProgram& program,
    const GLVertexArrays& vertex_array,
    const GLBuffers& vertex_buffer,
    const float start_x,
    const float start_y,
    const glm::vec3& color) const
{
    program.Use();
    program.SetUniform<UNIFORM_FLOAT_3>("COLOR", color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    m_Atlas.Bind(GL_TEXTURE_2D);

    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    vertex_array.Bind();

    const float vertices[6 * 4] = {
        start_x, start_y + height, 0.f, 0.f,
        start_x, start_y, 0.f, 1.f,
        start_x + width, start_y, 1.f, 1.f,

        start_x, start_y + height, 0.f, 0.f,
        start_x + width, start_y, 1.f, 1.f,
        start_x + width, start_y + height, 1.f, 0.f
    };

    vertex_buffer.Bind(GL_ARRAY_BUFFER);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
