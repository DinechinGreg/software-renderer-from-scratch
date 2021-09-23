#include <camera.h>
#include <renderer.h>
#include <vec3.h>

int main()
{
    camera main_camera = camera{1.0f, 1000.0f, 512, 512};
    main_camera.set_position(0.0f);
    renderer &global_renderer = renderer::get_instance();
    global_renderer.setup_scene();
    global_renderer.draw_to_file(main_camera);
    return 0;
}
