#include <graphics/camera.h>
#include <graphics/renderer/renderer.h>
#include <math/vec.h>

#include <dll_defines.h>

int main()
{
    // Initialize a camera object
    Camera main_camera = Camera{1.0f, 1000.0f, 512, 512};
    main_camera.set_position(Vec3f{0.0f, 3.0f, -10.0f});
    // Initialize a renderer object
    Renderer& global_renderer = Renderer::get_instance();
    global_renderer.initialize(main_camera, Vec3f::zero());
    // Render the scene
    while (global_renderer.should_continue_render_loop())
        global_renderer.draw_scene();
    // Release created objects
    global_renderer.release();
    return 0;
}
