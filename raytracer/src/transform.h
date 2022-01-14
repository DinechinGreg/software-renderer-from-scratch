#pragma once

#include <math/vec.h>

class transform
{
  public:
    transform();
    transform(Vec3f const& position);
    virtual ~transform() = 0;
    transform(transform const& other) = default;
    transform& operator=(transform const& other) = default;

    virtual Vec3f const& get_position() const { return m_position; }
    virtual void set_position(Vec3f const& new_value) { m_position = new_value; }

  protected:
    Vec3f m_position; // Position of the object
};
