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
    DECLSPECIFIER Vec3f const& get_color() const { return m_color; }

    /**
     * @brief Computes the direction from the light to a given point, which depends on the light's type.
     * @param[in] point_position. Position of the point in which to compute lighting, resulting from an intersection between a ray and the scene's geometry.
     * @return The light direction, as a unit vector.
     */
    DECLSPECIFIER Unit_Vec3f compute_light_direction(Vec3f const& point_position) const;

    /**
     * @brief Computes the distance from the light to a given point, which depends on the light's type.
     * @param[in] point_position. Position of the point in which to compute lighting, resulting from an intersection between a ray and the scene's geometry.
     * @return The light distance.
     */
    DECLSPECIFIER float compute_light_distance(Vec3f const& point_position) const;

    /**
     * @brief Computes the radiance of the light at a given distance.
     * @param[in] light_distance. Distance separating the light from the surface point in which to compute the light's contribution.
     * @return The radiance, depending on the light's color, intensity, and eventual attenuation due to distance.
     */
    DECLSPECIFIER Vec3f compute_radiance(float const& light_distance) const;

  protected:
    Light_Type m_type;      // Type of light (ambient, directional, point)
    float m_intensity;      // Intensity of the light
    Vec3f m_color;          // Color of the light
    Unit_Vec3f m_direction; // Direction of the light (for directional lights)
};
