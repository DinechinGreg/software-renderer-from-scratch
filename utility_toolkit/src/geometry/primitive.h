#pragma once

#include "ray.h"

#include <math/vec.h>

#include <vector>

namespace geometry
{

class Primitive
{
  public:
    /**
     * @brief Computes the normal of the point at the given position on this primitive.
     * @param[in] position. Position of a point on this primitive.
     * @return The corresponding normal direction.
     */
    virtual Unit_Vec3f compute_normal_from_position_on_primitive(Vec3f const& position) const = 0;

    /**
     * @brief Computes the UV coordinates of the point at the given position on this primitive.
     * @param[in] position. Position of a point on this primitive.
     * @return The corresponding UV coordinates.
     */
    virtual Vec2f compute_uv_from_position_on_primitive(Vec3f const& position) const = 0;

    /**
     * @brief Computes the intersection between this primitive and a given ray, within a given range.
     * @param[in] ray. Ray, with origin and direction.
     * @param[in] near_limit. Near limit, as a distance from the ray's origin, at which to start looking for intersections.
     * @param[in] far_limit. Far limit, as a distance from the ray's origin, at which to stop looking for intersections.
     * @param[out] out_intersections. Output intersection points, as distances from the ray's origin, within the given range (in order of distance if both are within the range).
     */
    virtual void compute_intersection_with(Ray const& ray, float near_limit, float far_limit, std::vector<float>& out_intersections) const
    {
        // Compute the intersections and store them in the member variable
        out_intersections.clear();
        compute_intersection_with(ray, out_intersections);
        // Remove intersections outside of the given range
        auto i = 0;
        while (i < out_intersections.size())
        {
            if (out_intersections[i] < near_limit || out_intersections[i] > far_limit)
                out_intersections.erase(out_intersections.begin() + i);
            else
                i++;
        }
    }

  protected:
    /**
     * @brief Computes the intersection between this primitive and a given ray.
     * Computed intersection points are stored in the member variable, as distances from the ray's origin, within the given range (in order of distance if both are within the range).
     * @param[in] ray. Ray, with origin and direction.
     * @param[out] out_intersections. Output intersection points, as distances from the ray's origin, within the given range (in order of distance if both are within the range).
     */
    virtual void compute_intersection_with(Ray const& ray, std::vector<float>& out_intersections) const = 0;
};

} // namespace geometry
