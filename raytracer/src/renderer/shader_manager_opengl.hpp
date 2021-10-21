#pragma once

#include <utilities/defines.h>

#if defined(RENDERER_OPENGL)

#include <string>

class shader_manager_opengl
{
  public:
    static shader_manager_opengl& get_instance()
    {
        static shader_manager_opengl global_shader_manager{};
        return global_shader_manager;
    }

    /**
     * @brief Compiles a vertex, geometry, or fragment shader at the given path.
     * @param[in] type. Type of shader to compile (ex: GL_VERTEX_SHADER).
     * @param[in] filepath. Path to the shader source, including the name of the file (but not the extension, which is deduced automatically from the type).
     * @return The ID of the created shader object.
     */
    unsigned int compile_shader(unsigned int type, std::string const& filepath) const;

    /**
     * @brief Compiles and links a vertex/fragment program from the source files at the given path.
     * @param[in] filepath. Path to the shader sources, including the name of the files (but not the extension, which is deduced automatically from the type).
     * @return The ID of the created program object.
     */
    unsigned int create_program(std::string const& filepath) const;
};

#endif
