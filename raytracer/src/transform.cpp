#include "transform.h"

transform::transform()
    : transform{vec3f::zero()}
{
}

transform::transform(vec3f const& position)
    : m_position{position}
{
}

transform::~transform() {}
