#include "transform.h"

transform::transform()
    : transform{vec3<float>::zero()}
{
}

transform::transform(vec3<float> const& position)
    : m_position{position}
{
}

transform::~transform() {}
