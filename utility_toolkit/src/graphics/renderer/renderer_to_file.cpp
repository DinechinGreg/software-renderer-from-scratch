#include "renderer_to_file.h"

#if defined(RENDERER_TO_FILE)

#include <fstream>
#include <vector>

void Renderer_To_File::initialize(camera const& draw_camera, Vec3f const& background_color)
{
    Renderer_Base::initialize(draw_camera, background_color);
    m_has_drawn_scene = false;
}

bool Renderer_To_File::should_continue_render_loop() const { return !m_has_drawn_scene; }

void Renderer_To_File::draw_scene()
{
    // Launch the threads and wait for them to finish processing
    launch_pixel_loading_threads();
    for (auto& loading_thread : m_loading_threads)
        loading_thread.join();
    // Output the pixels to file
    std::ofstream ofs("./_build/output.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << m_framebuffer_width << " " << m_framebuffer_height << "\n255\n";
    for (auto row_it = m_framebuffer_height - 1; row_it >= 0; row_it--)
    {
        for (auto column_it = 0; column_it < m_framebuffer_width; column_it++)
        {
            auto const index = (row_it * m_framebuffer_height + column_it);
            auto const loaded_pixel = m_loaded_pixels.at(index);
            ofs << static_cast<unsigned char>(std::min(1.0f, loaded_pixel.x()) * 255) << static_cast<unsigned char>(std::min(1.0f, loaded_pixel.y()) * 255) << static_cast<unsigned char>(std::min(1.0f, loaded_pixel.z()) * 255);
        }
    }
    ofs.close();
    // Notify that we have finished drawing
    m_has_drawn_scene = true;
}

void Renderer_To_File::release() {}

#endif
