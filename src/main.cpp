#include <string>
#include <DesktopTown/Context.hpp>
#include <DesktopTown/FontContext.hpp>
#include <DesktopTown/Sprite.hpp>
#include <glm/ext.hpp>

class App final : public DesktopTown::Context
{
public:
    App() = default;

protected:
    void OnStart() override
    {
        int width, height;
        GetSize(width, height);
        m_Fonts.Init(width, height);
        m_Fonts.LoadFont("font/Gothic3.ttf", 0, 48);

        m_Sprite.Load("image/town_building_01.png");
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

private:
    DesktopTown::FontContext m_Fonts;
    DesktopTown::Sprite m_Sprite;
};

int main()
{
    App app;
    app.Start();
    app.Stop();
}
