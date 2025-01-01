#pragma once

#include <map>
#include <string>
#include <DesktopTown/GL.hpp>
#include <freetype/freetype.h>
#include <glm/glm.hpp>

#include "FontContext.hpp"
#include "FontContext.hpp"

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
        FontContext();
        ~FontContext();

        void Init(GLProgram&& program, GLVertexArray&& vertex_array, GLBuffer&& vertex_buffer);

        void LoadFont(const std::string& filename, FT_UInt width = 0, FT_UInt height = 0, FT_Long index = 0);
        void DrawText(const std::wstring& text, float start_x, float start_y, float scale, const glm::vec3& color);
        void DrawAtlas(float start_x, float start_y, float scale, const glm::vec3& color);

    private:
        FT_Library m_FT{};

        std::map<wchar_t, Character> m_CharMap;

        GLTexture m_Atlas;
        GLProgram m_Program;
        GLVertexArray m_VertexArray;
        GLBuffer m_VertexBuffer;
    };
}
