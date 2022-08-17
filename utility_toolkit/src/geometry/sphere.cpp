#include "sphere.h"

#include "ray.h"

#include <math/math.h>
#include <math/vec.h>

#include <cmath>

namespace geometry
{

Unit_Vec3f Sphere::compute_normal_from_position_on_primitive(Vec3f const& position) const { return (position - m_origin).normalize(); }

Vec2f Sphere::compute_uv_from_position_on_primitive(Vec3f const& position) const
{
    auto const normal = compute_normal_from_position_on_primitive(position);
    auto const u = atan2(normal.x(), normal.z()) / (2.0f * math::pi) + 0.5f;
    auto const v = normal.y() * 0.5f + 0.5f;
    return Vec2f{u, v};
}

void Sphere::compute_intersection_with(Ray const& ray, culling::Type culling, std::vector<float>& out_intersections) const
{
    // Compute the discriminant based on the parametric equations of ray and sphere
    auto const& ray_direction = ray.get_direction();
    auto const center_to_origin = ray.get_origin() - m_origin;
    float const a = ray_direction.dot(ray_direction);
    float const b = 2 * center_to_origin.dot(ray_direction);
    float const c = center_to_origin.dot(center_to_origin) - m_radius * m_radius;
    float const discriminant = b * b - 4 * a * c;
    bool output_bool = (discriminant >= 0);

    // If the discriminant is strictly positive, there are two intersections
    if (discriminant > 0)
    {
        float first = (-b - std::sqrt(discriminant)) / (2 * a);
        float second = (-b + std::sqrt(discriminant)) / (2 * a);
        if (culling != culling::Type::FrontFace)
            out_intersections.push_back(first);
        if (culling != culling::Type::BackFace)
            out_intersections.push_back(second);
    }
    // If it is equal to zero, there is only one (border case)
    else if (discriminant == 0)
    {
        out_intersections.push_back(-b / (2 * a));
    }
}

} // namespace geometry
