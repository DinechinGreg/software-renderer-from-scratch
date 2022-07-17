#include "sphere.h"

#include <graphics/material.h>
#include <math/math.h>
#include <math/vec.h>

#include <cmath>

Vec2f Sphere::compute_uv_from_normal(Unit_Vec3f const& normal)
{
    auto const u = atan2(normal.x(), normal.z()) / (2.0f * math::pi) + 0.5f;
    auto const v = normal.y() * 0.5f + 0.5f;
    return Vec2f{u, v};
}
