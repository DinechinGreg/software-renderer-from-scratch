#pragma once

#include <graphics/transform.h>

#include <dll_defines.h>

class Camera : public Transform
{
  public:
    DECLSPECIFIER Camera(float near_plane = 0.1f, float far_plane = 1000.0f, int width = 256, int height = 256);
    DECLSPECIFIER ~Camera() = default;
    DECLSPECIFIER Camera(Camera const& other) = default;
    DECLSPECIFIER Camera& operator=(Camera const& other) = default;

    DECLSPECIFIER float const& get_near() const { return m_near; }
    DECLSPECIFIER float const& get_far() const { return m_far; }
    DECLSPECIFIER int const& get_width() const { return m_width; }
    DECLSPECIFIER int const& get_height() const { return m_height; }

  private:
    float m_near; // Near plane of the camera
    float m_far;  // Far plane of the camera
    int m_width;  // Width of the associated framebuffer
    int m_height; // Height of the associated framebuffer
};
