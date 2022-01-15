#pragma once

#include <graphics/renderer/renderer_defines.h>

#if defined(RENDERER_TO_FILE)

#include "renderer_base.h"

#include <graphics/camera.h>

#include <vector>

class Renderer_To_File : public Renderer_Base
{
  public:
    DECLSPECIFIER static Renderer_To_File& get_instance()
    {
        static Renderer_To_File global_renderer;
        return global_renderer;
    }

    DECLSPECIFIER void initialize(Camera const& draw_camera, Vec3f const& background_color) override;
    DECLSPECIFIER bool should_continue_render_loop() const override;
    DECLSPECIFIER void draw_scene() override;
    DECLSPECIFIER void release() override;

  private:
    bool m_has_drawn_scene; // True if the scene has already been drawn once, false otherwise
};

#endif
