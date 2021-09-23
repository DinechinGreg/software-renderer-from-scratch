#pragma once

#include <transform.h>

class camera : public transform
{
  public:
    camera() = default;
    camera(float near, float far, int width, int height);
    ~camera() = default;
    camera(camera const &other) = default;
    camera &operator=(camera const &other) = default;

    float const &get_near() const
    {
        return m_near;
    }
    float const &get_far() const
    {
        return m_far;
    }
    int const &get_width() const
    {
        return m_width;
    }
    int const &get_height() const
    {
        return m_height;
    }

  private:
    float m_near;
    float m_far;
    int m_width;
    int m_height;
};
