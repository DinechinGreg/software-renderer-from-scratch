#pragma once

#include <vec3.h>

class transform
{
  public:
    transform();
    transform(vec3<float> const &position);
    virtual ~transform() = 0;
    transform(transform const &other) = default;
    transform &operator=(transform const &other) = default;

    virtual vec3<float> const &get_position() const
    {
        return m_position;
    }
    virtual void set_position(vec3<float> const &new_value)
    {
        m_position = new_value;
    }

  protected:
    vec3<float> m_position;
};
