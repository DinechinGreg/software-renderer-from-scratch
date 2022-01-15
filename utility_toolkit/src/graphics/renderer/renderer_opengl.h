#pragma once

#include <graphics/renderer/renderer_defines.h>

#if defined(RENDERER_OPENGL)

#include "renderer_base.h"

#include <math/vec.h>

struct GLFWwindow;

class Renderer_OpenGL : public Renderer_Base
{
  public:
    DECLSPECIFIER static Renderer_OpenGL& get_instance()
    {
        static Renderer_OpenGL global_renderer;
        return global_renderer;
    }

    DECLSPECIFIER void initialize(Camera const& draw_camera, Vec3f const& background_color) override;
    DECLSPECIFIER bool should_continue_render_loop() const override;
    DECLSPECIFIER void draw_scene() override;
    DECLSPECIFIER void release() override;

    /**
     * @brief Modifies the framebuffer's size on receiving a callback from the window object.
     * Note that we constrain the width/height ratio of the framebuffer to stay the same.
     * @param[in] window. The window object being resized.
     * @param[in] width. The new proposed width for the window.
     * @param[in] height. The new proposed height for the window.
     */
    DECLSPECIFIER void change_framebuffer_size(GLFWwindow* window, int width, int height);

  private:
    DECLSPECIFIER Renderer_OpenGL();

    /**
     * @brief Initializes a GLFWwindow in which to display the application.
     */
    DECLSPECIFIER void initialize_window();

    /**
     * @brief Initializes OpenGL objects required to perform rendering of the image into a fullscreen quad.
     */
    DECLSPECIFIER void initialize_fullscreen_quad_rendering();

    /**
     * @brief Performs actions when the user presses a key.
     */
    DECLSPECIFIER void process_input();

    GLFWwindow* m_window;             // Main window of the application
    unsigned int m_vao_id;            // ID of the vertex array object used for fullscreen quad rendering
    unsigned int m_vbo_id;            // ID of the vertex buffer object used for fullscreen quad rendering
    unsigned int m_ebo_id;            // ID of the element buffer object used for fullscreen quad rendering
    unsigned int m_shader_program_id; // ID of the shader program object used for fullscreen quad rendering
    unsigned int m_texture_id;        // ID of the texture object used for fullscreen quad rendering
};

#endif
