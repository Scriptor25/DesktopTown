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

        void LoadFont(const std::string& filename, FT_UInt width = 0, FT_UInt height = 0, FT_Long index = 0);
        void DrawText(
            const GLProgram& program,
            const GLVertexArrays& vertex_array,
            const GLBuffers& vertex_buffer,
            const ::std::wstring& text,
            float start_x,
            float start_y,
            float scale,
            const glm::vec3& color);
        void DrawAtlas(
            const GLProgram& program,
            const GLVertexArrays& vertex_array,
            const GLBuffers& vertex_buffer,
            float start_x,
            float start_y,
            const glm::vec3& color) const;

    private:
        FT_Library m_FT;
        GLTextures m_Atlas;
        std::map<wchar_t, Character> m_CharMap;
    };
}
