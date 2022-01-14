#pragma once

#include <math/vec.h>

class Transform
{
  public:
    Transform();
    Transform(Vec3f const& position);
    virtual ~Transform() = 0;
    Transform(Transform const& other) = default;
    Transform& operator=(Transform const& other) = default;

    virtual Vec3f const& get_position() const { return m_position; }
    virtual void set_position(Vec3f const& new_value) { m_position = new_value; }

  protected:
    Vec3f m_position; // Position of the object
};
