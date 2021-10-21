#pragma once

#include "vec.h"

namespace math
{

/**
 * @brief Computes the reflected ray obtained by reflecting the given ray around the given direction.
 * @param[in] ray. Incident ray from which to compute a reflected ray.
 * @param[in] around. Direction around which to reflect the ray, as a unit vector.
 * @param[in] precomputed_dot. (Optional) Precomputed value of the dot product between incident ray and direction.
 * @return The reflected ray.
 */
static vec3f const compute_reflected_ray(vec3f const& ray, unit_vec3f const& around, float const* precomputed_dot = nullptr)
{
    float dot = (precomputed_dot == nullptr) ? ray.dot(around) : *precomputed_dot;
    return 2.0f * dot * around - ray;
}

/**
 * @brief Computes the intersection between a given ray and a given sphere.
 * @param[in] ray_origin. Point of origin of the ray.
 * @param[in] ray_direction. Direction of the ray, as a unit vector.
 * @param[in] sphere_position. Position of the center of the sphere.
 * @param[in] sphere_radius. Radius of the sphere.
 * @param[out] intersections. Output intersections within the given range (in order of distance if both are within the range).
 * @param[in] near. (Optional) Near limit, at which to start looking for intersections.
 * @param[in] far. (Optional) Far limit, at which to stop looking for intersections.
 * @return True if there was at least one intersection in the given range, false otherwise.
 */
static bool compute_ray_sphere_intersection(vec3f const& ray_origin, unit_vec3f const& ray_direction, vec3f const& sphere_position, float sphere_radius, std::vector<float>& intersections, float const* near, float const* far)
{
    // Clear the output vector
    intersections.clear();

    // Compute the discriminant based on the parametric equations of ray and sphere
    vec3f const& center_to_origin = ray_origin - sphere_position;
    float a = ray_direction.dot(ray_direction);
    float b = 2 * center_to_origin.dot(ray_direction);
    float c = center_to_origin.dot(center_to_origin) - sphere_radius * sphere_radius;
    float discriminant = b * b - 4 * a * c;
    bool output_bool = (discriminant >= 0);

    // If the discriminant is strictly positive, there are two intersections
    if (discriminant > 0)
    {
        float first = (-b - std::sqrt(discriminant)) / (2 * a);
        float second = (-b + std::sqrt(discriminant)) / (2 * a);
        intersections.push_back(first);
        intersections.push_back(second);
    }
    // If it is equal to zero, there is only one (border case)
    else if (discriminant == 0)
    {
        intersections.push_back(-b / (2 * a));
    }

    // If there is a provided range within which to check for the intersections, verify whether the computed intersections are within the range
    if (output_bool && near != nullptr && far != nullptr)
    {
        bool is_in_range = (intersections[0] >= *near && intersections[0] <= *far);
        if (!is_in_range && intersections.size() > 1)
        {
            is_in_range = (intersections[1] >= *near && intersections[1] <= *far);
            if (is_in_range)
                std::swap(intersections[0], intersections[1]);
        }
        output_bool = is_in_range;
    }

    // Return true if there was at least one intersection, false otherwise
    return output_bool;
}

} // namespace math
