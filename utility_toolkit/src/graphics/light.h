#pragma once

#include <graphics/transform.h>
#include <math/vec.h>

#include <dll_defines.h>

enum class DECLSPECIFIER Light_Type
{
    ambient,
    directional,
    point
};

class Light : public Transform
{
  public:
    DECLSPECIFIER Light();
    DECLSPECIFIER Light(Light_Type type, float intensity, Vec3f const& position_or_direction = Vec3f::zero(), Vec3f const& color = Vec3f::one());
    DECLSPECIFIER ~Light() = default;
    DECLSPECIFIER Light(Light const& other) = default;
    DECLSPECIFIER Light& operator=(Light const& other) = default;

    DECLSPECIFIER Light_Type const& get_type() const { return m_type; }
    DECLSPECIFIER float const& get_intensity() const { return m_intensity; }
    DECLSPECIFIER Vec3f const& get_direction() const { return m_direction; }

    /**
     * @brief Computes the light's contribution to lighting intensity in a given point.
     * @param[in] point_position. Position of the point in which to compute lighting, resulting from an intersection between a ray and the scene's geometry.
     * @param[in] point_distance. Distance separating the point from the ray's origin, used to have a notion of the precision of the intersection position.
     * @param[in] point_normal. Normal direction of the surface in the given point, as a unit vector.
     * @param[in] view_direction. Direction from the point to the origin of the ray, as a unit vector.
     * @param[in] specular_intensity. Intensity factor to use for the computation of the specular component.
     * @return The lighting intensity in the given point.
     */
    DECLSPECIFIER float apply_lighting(Vec3f const& point_position, float point_distance, Unit_Vec3f const& point_normal, Unit_Vec3f const& view_direction, float const& specular_intensity) const;

  protected:
    Light_Type m_type;      // Type of light (ambient, directional, point)
    float m_intensity;      // Intensity of the light
    Vec3f m_color;          // Color of the light
    Unit_Vec3f m_direction; // Direction of the light (for directional lights)
};
