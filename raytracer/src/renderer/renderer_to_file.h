#pragma once

#include <utilities/defines.h>

#if defined(RENDERER_TO_FILE)

#include "renderer_base.h"

#include <camera.h>

#include <vector>

class renderer_to_file : public renderer_base
{
  public:
    static renderer_to_file& get_instance()
    {
        static renderer_to_file global_renderer;
        return global_renderer;
    }

    void initialize(camera const& draw_camera, vec3f const& background_color) override;
    bool should_continue_render_loop() const override;
    void draw_scene() override;
    void release() override;

  private:
    bool m_has_drawn_scene; // True if the scene has already been drawn once, false otherwise
};

#endif
