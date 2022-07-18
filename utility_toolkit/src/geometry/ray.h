#pragma once

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
    Ray reflect(Unit_Vec3f const& around, float const* precomputed_dot = nullptr)
    {
        float dot = (precomputed_dot == nullptr) ? m_direction.dot(around) : *precomputed_dot;
        Unit_Vec3f reflected_direction = 2.0f * dot * around - m_direction;
        return Ray{m_origin, reflected_direction};
    }

  private:
    Vec3f m_origin;         // Origin of the ray
    Unit_Vec3f m_direction; // Direction of the ray
};

} // namespace geometry
