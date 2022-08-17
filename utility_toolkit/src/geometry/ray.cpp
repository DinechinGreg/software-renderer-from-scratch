#include "ray.h"

#include <math/math.h>

namespace geometry
{

Ray Ray::reflect(Unit_Vec3f const& around, float const* precomputed_dot) const
{
    float dot = (precomputed_dot == nullptr) ? m_direction.dot(around) : *precomputed_dot;
    Unit_Vec3f reflected_direction = 2.0f * dot * around - m_direction;
    return Ray{m_origin, reflected_direction};
}

float Ray::compute_intersection_with_plane(Unit_Vec3f const& plane_normal, float const plane_constant, culling::Type culling) const
{
    // Return negative infinity if there is no intersection because the plane is parallel to the ray, or because it faces the ray in a way that is impacted by culling
    auto const direction_dot_plane_normal = m_direction.dot(plane_normal);
    auto const value_to_check = (culling == culling::Type::BackFace) ? -direction_dot_plane_normal : (culling == culling::Type::FrontFace) ? direction_dot_plane_normal : std::abs(direction_dot_plane_normal);
    if (value_to_check <= math::numeric_epsilon())
        return -math::numeric_infinity();
    // Compute and return the intersection with the plane, as a (signed) distance from the ray's origin
    float const out_distance_along_ray = -(plane_normal.dot(m_origin) + plane_constant) / direction_dot_plane_normal;
    return out_distance_along_ray;
}

} // namespace geometry
