#pragma once

#include <math/vec.h>

class transform
{
  public:
    transform();
    transform(vec3f const& position);
    virtual ~transform() = 0;
    transform(transform const& other) = default;
    transform& operator=(transform const& other) = default;

    virtual vec3f const& get_position() const { return m_position; }
    virtual void set_position(vec3f const& new_value) { m_position = new_value; }

  protected:
    vec3f m_position;
};
