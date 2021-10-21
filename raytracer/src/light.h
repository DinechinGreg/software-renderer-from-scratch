#pragma once

#include <math/vec.h>
#include <transform.h>

enum class light_type
{
    ambient,
    directional,
    point
};

class light : public transform
{
  public:
    light();
    light(light_type type, float intensity, vec3f const& position_or_direction = vec3f::zero(), vec3f const& color = vec3f::one());
    ~light() = default;
    light(light const& other) = default;
    light& operator=(light const& other) = default;

    light_type const& get_type() const { return m_type; }
    float const& get_intensity() const { return m_intensity; }
    vec3f const& get_direction() const { return m_direction; }

    /**
     * @brief Computes the light's contribution to lighting intensity in a given point.
     * @param[in] point_position. Position of the point in which to compute lighting, resulting from an intersection between a ray and the scene's geometry.
     * @param[in] point_distance. Distance separating the point from the ray's origin, used to have a notion of the precision of the intersection position.
     * @param[in] point_normal. Normal direction of the surface in the given point, as a unit vector.
     * @param[in] view_direction. Direction from the point to the origin of the ray, as a unit vector.
     * @param[in] specular_intensity. Intensity factor to use for the computation of the specular component.
     * @return The lighting intensity in the given point.
     */
    float apply_lighting(vec3f const& point_position, float point_distance, unit_vec3f const& point_normal, unit_vec3f const& view_direction, float const& specular_intensity) const;

  protected:
    light_type m_type;      // Type of light (ambient, directional, point)
    float m_intensity;      // Intensity of the light
    vec3f m_color;          // Color of the light
    unit_vec3f m_direction; // Direction of the light (for directional lights)
};
