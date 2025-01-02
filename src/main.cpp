#include <fstream>
#include <string>
#include <DesktopTown/Context.hpp>
#include <DesktopTown/FontContext.hpp>
#include <DesktopTown/GL.hpp>
#include <DesktopTown/Sprite.hpp>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

static std::string read_file(const std::string& filename)
{
    std::ifstream stream(filename, std::ios_base::in | std::ios_base::ate);
    std::string data(stream.tellg(), 0);
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), static_cast<std::streamsize>(data.size()));
    return std::move(data);
}

class App final : public DesktopTown::Context
{
public:
    App()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto program = DesktopTown::GLProgram::Create();
        auto vertex_array = DesktopTown::GLVertexArray::Create();
        auto vertex_buffer = DesktopTown::GLBuffer::Create();

        {
            const auto shader = DesktopTown::GLShader::Create(GL_VERTEX_SHADER);
            const auto source = read_file("shader/text/vertex.glsl");
            shader.SetSource(source);
            shader.Compile();
            program.Attach(shader);
        }
        {
            const auto shader = DesktopTown::GLShader::Create(GL_FRAGMENT_SHADER);
            const auto source = read_file("shader/text/fragment.glsl");
            shader.SetSource(source);
            shader.Compile();
            program.Attach(shader);
        }
        program.Link();
        program.Validate();

        int width, height;
        GetSize(width, height);
        const auto fw = static_cast<float>(width);
        const auto fh = static_cast<float>(height);
        const auto projection = glm::ortho(0.f, fw, 0.f, fh);
        program.SetUniformMatrix<DesktopTown::UNIFORM_MATRIX_4_4>("PROJECTION", 1, GL_FALSE, &projection[0][0]);

        vertex_array.Bind();
        vertex_buffer.Bind(GL_ARRAY_BUFFER);

        vertex_buffer.Data<GLfloat>(6 * 4, GL_DYNAMIC_DRAW);

        vertex_array.Stride(4 * sizeof(float));
        vertex_array.VertexAttrib<float>(2, GL_FLOAT, GL_FALSE);
        vertex_array.VertexAttrib<float>(2, GL_FLOAT, GL_FALSE);

        m_Fonts.Init(std::move(program), std::move(vertex_array), std::move(vertex_buffer));

        m_Sprite.Load("image/town_building_01.png");
    }

protected:
    void OnStart() override
    {
        m_Fonts.LoadFont("font/Gothic3.ttf", 0, 48);
    }

    void OnUpdate() override
    {
        m_Fonts.DrawText(
            L"Hello World!",
            0.f,
            48.f,
            1.f,
            {1.f, 1.f, 0.f});
        m_Fonts.DrawText(
            L"This Ä is Ö some Ü more ß text.",
            0.f,
            0.f,
            .5f,
            {0.f, 1.f, 1.f});
        m_Fonts.DrawAtlas(0.f, 96.f, .2f, {1.f, 0.f, 1.f});
    }

    DesktopTown::FontContext m_Fonts;
    DesktopTown::Sprite m_Sprite;
};

int main()
{
    App app;
    app.Start();
    app.Stop();
}
