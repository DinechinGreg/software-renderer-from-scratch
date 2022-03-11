#pragma once

#include <math/vec.h>

class Texture
{
  public:
    Texture(Vec3f tmp)
        : m_tmp{tmp}
    {
    }

    Vec3f sample(Vec2f uv) const { return m_tmp; }

  private:
    Vec3f m_tmp; // TODO
};
