#include <camera.h>
#include <renderer.h>
#include <vec3.h>

int main()
{
    camera main_camera = camera{1.0f, 1000.0f, 256, 256};
    main_camera.set_position(vec3<float>::zero());
    renderer& global_renderer = renderer::get_instance();
    global_renderer.setup_scene();
    global_renderer.draw_to_file(main_camera);
    return 0;
}
