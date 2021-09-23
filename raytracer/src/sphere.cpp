#include "sphere.h"

#include <vec3.h>

#include <cmath>

sphere::sphere(vec3<float> const &center, float radius, vec3<float> const &color, float const &specular_intensity)
    : transform{center}, m_radius{radius}, m_color{color}, m_specular_intensity{specular_intensity}
{
}

sphere::sphere() : sphere{vec3{0.0f}, 1.0f, vec3{0.0f}, -1.0f}
{
}

bool sphere::intersects_ray(vec3<float> const &origin, vec3<float> const &ray, std::vector<float> &intersections) const
{
    intersections.clear();
    vec3<float> const &unit_ray = ray.normalize();
    vec3<float> const &center_to_origin = origin - m_position;

    float a = unit_ray.dot(unit_ray);
    float b = 2 * center_to_origin.dot(unit_ray);
    float c = center_to_origin.dot(center_to_origin) - m_radius * m_radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0)
    {
        float first = (-b - std::sqrt(discriminant)) / (2 * a);
        float second = (-b + std::sqrt(discriminant)) / (2 * a);
        if (first > second)
            std::swap(first, second);
        intersections.push_back(first);
        intersections.push_back(second);
    }
    else if (discriminant == 0)
    {
        intersections.push_back(-b / (2 * a));
    }

    return (intersections.size() == 1 || intersections.size() == 2);
}
