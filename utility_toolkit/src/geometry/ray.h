#pragma once

#include <graphics/culling.h>
#include <math/vec.h>

namespace geometry
{

class Ray
{
  public:
    Ray(Vec3f const& origin, Unit_Vec3f const& direction)
        : m_origin{origin}
        , m_direction{direction}
    {
    }
    ~Ray() = default;
    Ray(Ray const& other) = default;
    Ray& operator=(Ray const& other) = default;

    Vec3f const& get_origin() const { return m_origin; }
    Unit_Vec3f const& get_direction() const { return m_direction; }

    /**
     * @brief Computes the reflected ray obtained by reflecting this ray around the given direction.
     * @param[in] around. Direction around which to reflect the ray, as a unit vector.
     * @param[in] precomputed_dot. (Optional) Precomputed value of the dot product between incident ray and direction.
     * @return The reflected ray.
     */
    Ray reflect(Unit_Vec3f const& around, float const* precomputed_dot = nullptr) const;

    /**
     * @brief Computes the intersection between this ray and the given plane.
     * @param[in] plane_normal. Normal of the plane.
     * @param[in] plane_constant. Constant that determines the plane's position.
     * @param[in] culling. Type of culling to apply to determine whether there is intersection with the plane.
     * @return
     */
    float compute_intersection_with_plane(Unit_Vec3f const& plane_normal, float const plane_constant, culling::Type culling) const;

  private:
    Vec3f m_origin;         // Origin of the ray
    Unit_Vec3f m_direction; // Direction of the ray
};

} // namespace geometry
