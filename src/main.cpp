#include <string>
#include <DesktopTown/Context.hpp>
#include <DesktopTown/FontContext.hpp>
#include <DesktopTown/Sprite.hpp>
#include <glm/ext.hpp>

class App final : public DesktopTown::Context
{
public:
    App()
        : m_Fonts(this),
          m_Sprite(this)
    {
    }

protected:
    void OnStart() override
    {
        m_Fonts.LoadFont("font/Gothic3.ttf", 0, 48);
        m_Sprite.Load("image/town_building_01.png");
    }

    void OnUpdate() override
    {
        m_Sprite.Draw(m_AnimationFrame, 0.f, 90.f, 8.f);

        if (const auto current = glfwGetTime(); current - m_Time > 0.5f)
        {
            m_Time = current;
            m_AnimationFrame++;
        }

        m_Fonts.DrawText(
            L"Hello World!",
            0.f,
            40.f,
            1.f,
            {1.f, 1.f, 0.f});
        m_Fonts.DrawText(
            L"This Ä is Ö some Ü more ß text.",
            0.f,
            0.f,
            .5f,
            {0.f, 1.f, 1.f});
    }

private:
    DesktopTown::FontContext m_Fonts;
    DesktopTown::Sprite m_Sprite;

    unsigned m_AnimationFrame = 0;

    double m_Time = 0.f;
};

int main()
{
    App app;
    app.Start();
    app.Stop();
}
