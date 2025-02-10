#include <filesystem>
#include <ranges>
#include <span>
#include <DesktopTown/Context.hpp>
#include <DesktopTown/Error.hpp>
#include <DesktopTown/FileUtil.hpp>
#include <DesktopTown/FontContext.hpp>
#include <DesktopTown/VecView.hpp>
#include <DesktopTown/GL/GLQuery.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

DesktopTown::FontContext::FontContext(Context *context)
    : m_Context(context)
{
    FT_Init_FreeType(&m_FT);

    std::vector<ShaderInfo> shader_infos;
    shader_infos.emplace_back("text.vertex.gl", "shader/text/vertex.glsl", GL_VERTEX_SHADER);
    shader_infos.emplace_back("text.fragment.gl", "shader/text/fragment.glsl",GL_FRAGMENT_SHADER);
    m_Material = Material("text.gl", std::move(shader_infos));

    if (m_Material.Load())
        Error("Failed to load text material.");

    m_AtlasMesh.SetVertices(
        {
            {{0, 0}, {0, 1}},
            {{0, 1}, {0, 0}},
            {{1, 1}, {1, 0}},
            {{1, 0}, {1, 1}},
        });
    m_AtlasMesh.SetIndices(
        {
            0u,
            1u,
            2u,
            2u,
            3u,
            0u,
        });
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

void DesktopTown::FontContext::LoadFont(
    const std::string &filename,
    const FT_UInt width,
    const FT_UInt height,
    const FT_Long index)
{
    FT_Face face;
    FT_New_Face(m_FT, filename.c_str(), index, &face);
    FT_Set_Pixel_Sizes(face, width, height);

    unsigned max_width = 0, max_height = 0;
    for (auto c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(face, c, FT_LOAD_DEFAULT);

        const auto c_width = face->glyph->bitmap.width;
        const auto c_height = face->glyph->bitmap.rows;

        if (c_width > max_width)
            max_width = c_width;
        if (c_height > max_height)
            max_height = c_height;
    }

    m_CharMap.clear();

    for (auto c = MIN_CHAR; c < MAX_CHAR; ++c)
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

    m_Width = static_cast<GLsizei>(NUM_TILES_RT * max_width);
    m_Height = static_cast<GLsizei>(NUM_TILES_RT * max_height);

    m_Atlas = GLTexture::Create(GL_TEXTURE_2D);
    m_Atlas.Storage(1, GL_R32F, m_Width, m_Height);
    m_Atlas.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Atlas.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_Atlas.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_Atlas.SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (auto c = MIN_CHAR; c < MAX_CHAR; ++c)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        const auto c_width = static_cast<GLsizei>(face->glyph->bitmap.width);
        const auto c_height = static_cast<GLsizei>(face->glyph->bitmap.rows);

        if (!c_width || !c_height || !face->glyph->bitmap.buffer)
            continue;

        const auto off = c - MIN_CHAR;
        const auto i = static_cast<GLint>(off % NUM_TILES_RT * max_width);
        const auto j = static_cast<GLint>(off / NUM_TILES_RT * max_height);

        m_Atlas.SubImage(0, i, j, c_width, c_height, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    }

    FT_Done_Face(face);
}

void DesktopTown::FontContext::DrawText(
    const std::wstring &text,
    const float start_x,
    const float start_y,
    const float font_scale,
    const glm::vec3 &color)
{
    if (!m_TextMeshCache.contains(text))
    {
        std::vector<Vertex> vertices(text.size() * 4);
        std::vector<GLuint> indices(text.size() * 6);

        auto x = 0.f;
        for (unsigned i = 0; i < text.size(); ++i)
        {
            auto &c = text[i];
            auto &[
                texture_,
                size_,
                bearing_,
                advance_
            ] = m_CharMap[c];

            const auto xpos = x + static_cast<float>(bearing_.x);
            const auto ypos = -static_cast<float>(size_.y - bearing_.y);

            const auto w = static_cast<float>(size_.x);
            const auto h = static_cast<float>(size_.y);

            const auto base = i * 4;

            VecView(vertices, i * 4) = {
                Vertex{{xpos + 0, ypos + 0}, {texture_.x, texture_.w}},
                Vertex{{xpos + 0, ypos + h}, {texture_.x, texture_.y}},
                Vertex{{xpos + w, ypos + h}, {texture_.z, texture_.y}},
                Vertex{{xpos + w, ypos + 0}, {texture_.z, texture_.w}},
            };
            VecView(indices, i * 6) = {
                base + 0,
                base + 1,
                base + 2,
                base + 2,
                base + 3,
                base + 0,
            };

            x += static_cast<float>(advance_ >> 6);
        }

        auto &text_mesh = m_TextMeshCache[text];
        text_mesh.SetVertices(vertices);
        text_mesh.SetIndices(indices);
    }

    m_Atlas.BindUnit(0);

    const auto defer_material = m_Material->Bind();

    glm::mat4 model(1.f);
    model = translate(model, glm::vec3(start_x, start_y, 0.f));
    model = scale(model, glm::vec3(font_scale, font_scale, 1.f));
    auto &projection = m_Context->GetProjection();

    m_Material->SetUniform<Uniform_Float3>("COLOR", color.x, color.y, color.z);
    m_Material->SetUniformMatrix<Uniform_Matrix4x4>("MODEL", 1, GL_FALSE, &model[0][0]);
    m_Material->SetUniformMatrix<Uniform_Matrix4x4>("PROJECTION", 1, GL_FALSE, &projection[0][0]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_TextMeshCache[text].Draw();

    glDisable(GL_BLEND);
}

void DesktopTown::FontContext::DrawAtlas(
    const float start_x,
    const float start_y,
    const float font_scale,
    const glm::vec3 &color)
{
    m_Atlas.BindUnit(0);

    const auto defer_material = m_Material->Bind();

    glm::mat4 model(1.f);
    model = translate(model, glm::vec3(start_x, start_y, 0.f));
    model = scale(
        model,
        glm::vec3(
            static_cast<float>(m_Width) * font_scale,
            static_cast<float>(m_Height) * font_scale,
            1.f));
    auto &projection = m_Context->GetProjection();

    m_Material->SetUniform<Uniform_Float3>("COLOR", color.x, color.y, color.z);
    m_Material->SetUniformMatrix<Uniform_Matrix4x4>("MODEL", 1, GL_FALSE, &model[0][0]);
    m_Material->SetUniformMatrix<Uniform_Matrix4x4>("PROJECTION", 1, GL_FALSE, &projection[0][0]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_AtlasMesh.Draw();

    glDisable(GL_BLEND);
}
