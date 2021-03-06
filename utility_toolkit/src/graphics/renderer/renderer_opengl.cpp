#include "renderer_opengl.h"

#if defined(RENDERER_OPENGL)

#include <graphics/renderer/opengl_headers.h>
#include <graphics/renderer/shader_manager_opengl.hpp>

#include <array>
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

Renderer_OpenGL::Renderer_OpenGL()
    : Renderer_Base()
    , m_window{nullptr}
    , m_vao_id{0}
    , m_vbo_id{0}
    , m_ebo_id{0}
    , m_shader_program_id{0}
    , m_texture_id{0}
{
}

void Renderer_OpenGL::initialize(Camera const& draw_camera, Vec3f const& background_color)
{
    Renderer_Base::initialize(draw_camera, background_color);
    initialize_window();
    initialize_fullscreen_quad_rendering();
    launch_pixel_loading_threads();
}

bool Renderer_OpenGL::should_continue_render_loop() const { return (m_window == nullptr || !glfwWindowShouldClose(m_window)); }

void Renderer_OpenGL::draw_scene()
{
    // Check for user input
    process_input();

    // Clear the framebuffer's color
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a fullscreen quad with the given texture
    glUseProgram(m_shader_program_id);
    glBindVertexArray(m_vao_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    m_thread_guard.lock();
    if (m_loaded_pixels.size() > 0)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        for (auto& pixel : m_loaded_pixels)
        {
            auto const pixel_index = pixel.first;
            auto const pixel_rgb = pixel.second;
            auto const pixel_y = pixel_index / m_framebuffer_width;
            auto const pixel_x = pixel_index - (pixel_y * m_framebuffer_width);
            std::vector<float> pixel_rgb_value{pixel_rgb.x(), pixel_rgb.y(), pixel_rgb.z()};
            glTexSubImage2D(GL_TEXTURE_2D, 0, pixel_x, pixel_y, 1, 1, GL_RGB, GL_FLOAT, pixel_rgb_value.data());
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        m_loaded_pixels.clear();
    }
    m_thread_guard.unlock();

    // Swap buffers
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Renderer_OpenGL::release()
{
    Renderer_Base::release();

    glDeleteVertexArrays(1, &m_vao_id);
    glDeleteBuffers(1, &m_vbo_id);
    glDeleteBuffers(1, &m_ebo_id);
    glDeleteTextures(1, &m_texture_id);
    glDeleteProgram(m_shader_program_id);

    glfwTerminate();
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { Renderer_OpenGL::get_instance().change_framebuffer_size(window, width, height); }

void Renderer_OpenGL::initialize_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_framebuffer_width, m_framebuffer_height, "Raytracer", nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_framebuffer_width, m_framebuffer_height, 0, GL_RGB, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer_OpenGL::initialize_fullscreen_quad_rendering()
{
    m_shader_program_id = Shader_Manager_OpenGL::get_instance().create_program("../utility_toolkit/src/graphics/renderer/shaders/texture");

    float vertices[] = {
        // Clip-space positions
        // Texture UVs
        -1.0f, -1.0f, 0.0f, // Bottom left
        0.0f,  0.0f,        //
        -1.0f, 1.0f,  0.0f, // Top left
        0.0f,  1.0f,        //
        1.0f,  -1.0f, 0.0f, // Bottom right
        1.0f,  0.0f,        //
        1.0f,  1.0f,  0.0f, // Top right
        1.0f,  1.0f         //
    };
    auto const& vertex_stride = static_cast<int>(5 * sizeof(float));
    auto const* position_attribute_offset = reinterpret_cast<void*>(0);
    auto const* texture_attribute_offset = reinterpret_cast<void*>(3 * sizeof(float));

    unsigned int indices[] = {0, 1, 2, 3, 2, 1};

    glGenVertexArrays(1, &m_vao_id);
    glGenBuffers(1, &m_vbo_id);
    glGenBuffers(1, &m_ebo_id);

    glBindVertexArray(m_vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_stride, position_attribute_offset);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_stride, texture_attribute_offset);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer_OpenGL::process_input()
{
    // Enable closing the window on escape key press
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void Renderer_OpenGL::change_framebuffer_size(GLFWwindow* window, int width, int height)
{
    if (window == m_window && (m_framebuffer_width != width || m_framebuffer_height != height))
    {
        // Update window width and height values while ensuring the aspect ratio stays the same
        auto const height_on_width_ratio = m_framebuffer_height * 1.0f / m_framebuffer_width;
        auto width_mult_factor = 1.0f;
        if (m_framebuffer_width != width)
        {
            width_mult_factor = (width * 1.0f / m_framebuffer_width);
        }
        else if (m_framebuffer_height != height)
        {
            width_mult_factor = ((height / height_on_width_ratio) * 1.0f / m_framebuffer_width);
        }
        m_framebuffer_width = static_cast<int>(width_mult_factor * m_framebuffer_width);
        m_framebuffer_height = static_cast<int>(height_on_width_ratio * m_framebuffer_width);
        // Update the viewport and window size accordingly
        glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);
        glfwSetWindowSize(m_window, m_framebuffer_width, m_framebuffer_height);
    }
}

#endif
