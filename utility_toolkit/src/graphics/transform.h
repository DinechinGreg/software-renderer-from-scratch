#pragma once

#include <math/vec.h>

#include <dll_defines.h>

class Transform
{
  public:
    DECLSPECIFIER Transform();
    DECLSPECIFIER Transform(Vec3f const& position);
    DECLSPECIFIER virtual ~Transform() = 0;
    DECLSPECIFIER Transform(Transform const& other) = default;
    DECLSPECIFIER Transform& operator=(Transform const& other) = default;

    DECLSPECIFIER virtual Vec3f const& get_position() const { return m_position; }
    DECLSPECIFIER virtual void set_position(Vec3f const& new_value) { m_position = new_value; }

  protected:
    Vec3f m_position; // Position of the object
};
