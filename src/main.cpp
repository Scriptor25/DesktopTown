#include <fstream>
#include <string>
#include <DesktopTown/Context.hpp>
#include <DesktopTown/FontContext.hpp>
#include <DesktopTown/GL.hpp>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

static std::string read_file(const std::string& filename)
{
    std::ifstream stream(filename, std::ios_base::in | std::ios_base::ate);
    std::string data(stream.tellg(), 0);
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), data.size());
    return std::move(data);
}

static DesktopTown::GLProgram* program;
static DesktopTown::GLVertexArrays* vertex_array;
static DesktopTown::GLBuffers* vertex_buffer;
static DesktopTown::FontContext font_context;

static void on_start(DesktopTown::Context& ctx)
{
    font_context.LoadFont("font/Gothic3.ttf", 0, 48);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program = new DesktopTown::GLProgram();
    vertex_array = new DesktopTown::GLVertexArrays();
    vertex_buffer = new DesktopTown::GLBuffers();

    {
        const DesktopTown::GLShader shader(GL_VERTEX_SHADER);
        const auto source = read_file("shader/text/vertex.glsl");
        shader.SetSource(source);
        shader.Compile();
        program->Attach(shader);
    }
    {
        const DesktopTown::GLShader shader(GL_FRAGMENT_SHADER);
        const auto source = read_file("shader/text/fragment.glsl");
        shader.SetSource(source);
        shader.Compile();
        program->Attach(shader);
    }
    program->Link();
    program->Validate();

    int width, height;
    ctx.GetSize(width, height);
    const auto fw = static_cast<float>(width);
    const auto fh = static_cast<float>(height);
    const auto projection = glm::ortho(0.f, fw, 0.f, fh);
    program->SetUniformMatrix<DesktopTown::UNIFORM_MATRIX_4_4>("PROJECTION", 1, GL_FALSE, &projection[0][0]);

    vertex_array->Bind();
    vertex_buffer->Bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
}

static void on_update(DesktopTown::Context& ctx)
{
    font_context.DrawText(
        *program,
        *vertex_array,
        *vertex_buffer,
        "Hello World!",
        0.f,
        96.f,
        1.f,
        {1.f, 1.f, 1.f});
    font_context.DrawText(
        *program,
        *vertex_array,
        *vertex_buffer,
        "This is some more text.",
        0.f,
        0.f,
        1.f,
        {1.f, 1.f, 1.f});
}

static void on_stop(DesktopTown::Context& ctx)
{
    delete program;
    delete vertex_array;
    delete vertex_buffer;
}

int main()
{
    DesktopTown::Context({on_start, on_update, on_stop});
}
