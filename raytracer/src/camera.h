#pragma once

#include <transform.h>

class camera : public transform
{
  public:
    camera(float near = 0.1f, float far = 1000.0f, int width = 256, int height = 256);
    ~camera() = default;
    camera(camera const& other) = default;
    camera& operator=(camera const& other) = default;

    float const& get_near() const { return m_near; }
    float const& get_far() const { return m_far; }
    int const& get_width() const { return m_width; }
    int const& get_height() const { return m_height; }

  private:
    float m_near; // Near plane of the camera
    float m_far;  // Far plane of the camera
    int m_width;  // Width of the associated framebuffer
    int m_height; // Height of the associated framebuffer
};
