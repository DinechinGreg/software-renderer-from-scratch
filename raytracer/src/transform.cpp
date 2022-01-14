#include "transform.h"

transform::transform()
    : transform{Vec3f::zero()}
{
}

transform::transform(Vec3f const& position)
    : m_position{position}
{
}

transform::~transform() {}
