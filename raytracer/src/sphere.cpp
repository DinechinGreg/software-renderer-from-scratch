#include "sphere.h"

#include <math/vec.h>

#include <cmath>

sphere::sphere(Vec3f const& center, float radius, Vec3f const& color, float const& specular_intensity, float const& reflective_intensity)
    : transform{center}
    , m_radius{radius}
    , m_color{color}
    , m_specular_intensity{specular_intensity}
    , m_reflective_intensity{reflective_intensity}
{
}

sphere::sphere()
    : sphere{Vec3f::zero(), 1.0f, Vec3f::zero(), -1.0f, -1.0f}
{
}
