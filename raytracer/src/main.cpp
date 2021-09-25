#include <camera.h>
#include <math/vec.h>
#include <renderer.h>

int main()
{
    camera main_camera = camera{1.0f, 1000.0f, 256, 256};
    main_camera.set_position(vec3f::zero());
    renderer& global_renderer = renderer::get_instance();
    global_renderer.setup_scene();
    global_renderer.draw_to_file(main_camera);
    return 0;
}
