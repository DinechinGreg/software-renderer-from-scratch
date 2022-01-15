#include "shader_manager_opengl.hpp"

#if defined(RENDERER_OPENGL)

#include <filesystem/resource_manager.h>
#include <graphics/renderer/opengl_headers.h>

#include <iostream>
#include <string>

#if defined(_DEBUG)
static auto constexpr log_size = 512;
#endif

unsigned int Shader_Manager_OpenGL::compile_shader(unsigned int type, std::string const& filepath) const
{
    // Get the shader source from the filepath
    auto const& shader_source_string{Resource_Manager::get_instance().read_file(filepath)};
    char const* const shader_source{shader_source_string.data()};
    // Create and compile the shader
    auto const& shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);
#if defined(_DEBUG)
    // Check for compile errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[log_size];
        glGetShaderInfoLog(shader, log_size, nullptr, info_log);
        std::cout << "ERROR: " << type << " shader compilation failed with message: " << info_log << std::endl;
    }
#endif
    // Return the shader ID
    return shader;
}

unsigned int Shader_Manager_OpenGL::create_program(std::string const& filepath) const
{
    // Compile the vertex and fragment shaders
    auto const& vertex_shader = compile_shader(GL_VERTEX_SHADER, filepath + ".vtx");
    auto const& fragment_shader = compile_shader(GL_FRAGMENT_SHADER, filepath + ".frag");
    // Attach and link into a shader program
    auto const& shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
#if defined(_DEBUG)
    // Check for link errors
    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[log_size];
        glGetProgramInfoLog(shader_program, log_size, nullptr, info_log);
        std::cout << "ERROR: shader program linking failed with message: " << info_log << std::endl;
    }
#endif
    // Delete the vertex and fragment shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    // Return the shader program ID
    return shader_program;
}

#endif
