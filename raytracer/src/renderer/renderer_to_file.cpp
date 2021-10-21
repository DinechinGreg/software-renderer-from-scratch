#include "renderer_to_file.h"

#if defined(RENDERER_TO_FILE)

#include <fstream>
#include <vector>

void renderer_to_file::initialize(camera const& draw_camera, vec3f const& background_color)
{
    renderer_base::initialize(draw_camera, background_color);
    m_has_drawn_scene = false;
}

bool renderer_to_file::should_continue_render_loop() const { return !m_has_drawn_scene; }

void renderer_to_file::draw_scene()
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
        auto const& loaded_pixel_row = m_loaded_pixels[row_it];
        for (auto& pixel_value : loaded_pixel_row)
        {
            ofs << static_cast<unsigned char>(std::min(1.0f, pixel_value) * 255);
        }
    }
    ofs.close();
    // Notify that we have finished drawing
    m_has_drawn_scene = true;
}

void renderer_to_file::release() {}

#endif
