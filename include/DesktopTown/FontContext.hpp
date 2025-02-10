#pragma once

#include <map>
#include <string>
#include <DesktopTown/Material.hpp>
#include <DesktopTown/Mesh.hpp>
#include <DesktopTown/GL/GLTexture.hpp>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace DesktopTown
{
    class FontContext
    {
        struct Character
        {
            glm::vec4 Texture;
            glm::ivec2 Size;
            glm::ivec2 Bearing;
            FT_Pos Advance;
        };

    public:
        explicit FontContext(const Context *context);
        ~FontContext();

        void LoadFont(const std::string &filename, FT_UInt width = 0, FT_UInt height = 0, FT_Long index = 0);
        void DrawText(const std::wstring &text, float start_x, float start_y, float scale, const glm::vec3 &color);
        void DrawAtlas(float start_x, float start_y, float scalar, const glm::vec3 &color);

    private:
        FT_Library m_FT{};

        std::map<wchar_t, Character> m_CharMap;

        GLsizei m_Width{}, m_Height{};
        GLTexture m_Atlas;
        Material m_Material;

        Mesh m_TextMesh;
        Mesh m_AtlasMesh;
    };
}
