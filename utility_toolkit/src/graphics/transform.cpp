#include "transform.h"

Transform::Transform()
    : Transform{Vec3f::zero()}
{
}

Transform::Transform(Vec3f const& position)
    : m_position{position}
{
}

Transform::~Transform() {}
