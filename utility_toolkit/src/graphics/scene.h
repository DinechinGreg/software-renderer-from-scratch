#pragma once

#include <graphics/light.h>
#include <graphics/object.h>

#include <vector>

class Scene
{
  public:
    Scene() = default;
    ~Scene() = default;
    Scene(Scene const& other) = default;
    Scene& operator=(Scene const& other) = default;

    std::vector<Object> const& get_objects() const { return m_objects; }
    std::vector<Light> const& get_lights() const { return m_lights; }

    /**
     * @brief Sets up a default scene, with a grid of spheres and a point light.
     */
    void setup_default_scene();

  private:
    std::vector<Object> m_objects; // List of objects that compose the scene's geometry
    std::vector<Light> m_lights;   // List of lights that compose the scene's lighting
};
