#pragma once

#include <DesktopTown/Material.hpp>
#include <DesktopTown/Mesh.hpp>

namespace DesktopTown
{
    class Model
    {
    public:
        Model();

    private:
        Material m_Material;
        std::vector<Mesh> m_Meshes;
    };
}
