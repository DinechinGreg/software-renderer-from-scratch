#include "transform.h"

transform::transform() : transform{0.0f}
{
}

transform::transform(vec3<float> const &position) : m_position{position}
{
}

transform::~transform()
{
}
