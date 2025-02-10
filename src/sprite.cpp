#include <stb_image.h>
#include <DesktopTown/Context.hpp>
#include <DesktopTown/Error.hpp>
#include <DesktopTown/Sprite.hpp>
#include <glm/ext.hpp>

DesktopTown::Sprite::Sprite(Context *context)
    : m_Context(context)
{
    std::vector<ShaderInfo> shader_infos;
    shader_infos.emplace_back("sprite.vertex.gl", "shader/sprite/vertex.glsl", GL_VERTEX_SHADER);
    shader_infos.emplace_back("sprite.fragment.gl", "shader/sprite/fragment.glsl", GL_FRAGMENT_SHADER);
    m_Material = Material("sprite.gl", std::move(shader_infos));

    if (m_Material.Load())
        Error("Failed to load sprite material.");

    m_Mesh.SetVertices(
        {
            {{0, 0}, {0, 1}},
            {{0, 1}, {0, 0}},
            {{1, 1}, {1, 0}},
            {{1, 0}, {1, 1}},
        });

    m_Mesh.SetIndices(
        {
            0u,
            1u,
            2u,
            2u,
            3u,
            0u,
        });
}

void DesktopTown::Sprite::Load(const std::string &filename)
{
    int width, height;
    const auto pixels = stbi_load(filename.c_str(), &width, &height, nullptr, 4);

    if (!pixels)
    {
        Error("Failed to load image from file '{}': {}", filename, stbi_failure_reason());
        return;
    }

    m_Texture = GLTexture::Create(GL_TEXTURE_2D);
    m_Texture.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Texture.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_Texture.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_Texture.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_Texture.Storage(1, GL_RGBA32F, width, height);
    m_Texture.SubImage(0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);

    if (height % width != 0)
    {
        m_FrameScale = 0.f;
        m_FrameWidth = width;
        m_FrameHeight = height;
        m_NumFrames = 0;
    }
    else
    {
        m_FrameScale = static_cast<float>(width) / static_cast<float>(height);
        m_FrameWidth = static_cast<float>(width);
        m_FrameHeight = static_cast<float>(width);
        m_NumFrames = height / width;
    }
}

void DesktopTown::Sprite::Draw(unsigned frame, const float x, const float y, const float s)
{
    frame %= m_NumFrames;

    m_Texture.BindUnit(0);

    const auto defer_material = m_Material->Bind();

    glm::mat4 model(1.f);
    model = translate(model, glm::vec3(x, y, 0.f));
    model = scale(model, glm::vec3(s * m_FrameWidth, s * m_FrameHeight, 1.f));
    const auto &projection = m_Context->GetProjection();

    m_Material->SetUniformMatrix<Uniform_Matrix4x4>("MODEL", 1, GL_FALSE, &model[0][0]);
    m_Material->SetUniformMatrix<Uniform_Matrix4x4>("PROJECTION", 1, GL_FALSE, &projection[0][0]);

    m_Material->SetUniform<Uniform_Float2>("FRAME_SCALE", 1.f, m_FrameScale);
    m_Material->SetUniform<Uniform_Float2>("FRAME_OFFSET", 0.f, static_cast<float>(frame) * m_FrameScale);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Mesh.Draw();

    glDisable(GL_BLEND);
}
