#pragma once

#include <graphics/light.h>
#include <graphics/physically_based_rendering.h>
#include <graphics/texture.h>

#include <vector>

class Material
{
  public:
    Material(Vec3f albedo)
        : m_albedo_texture{albedo}
        , m_metallic_map{Vec3f{0.2f}}
        , m_roughness_map{Vec3f{0.1f}}
        , m_ambient_occlusion_map{Vec3f{0.1f}}
    {
    }

    Vec3f get_albedo(Vec2f const& uv) const { return m_albedo_texture.sample(uv).pow(2.2f); }
    float get_metallic(Vec2f const& uv) const { return m_metallic_map.sample(uv).x(); }
    float get_roughness(Vec2f const& uv) const { return m_roughness_map.sample(uv).x(); }
    float get_ambient_occlusion(Vec2f const& uv) const { return m_ambient_occlusion_map.sample(uv).x(); }
    Vec3f get_base_reflectivity(Vec3f const& albedo, float metallic) const { return math::linear_interpolation(Vec3f{dielectric_base_reflectivity}, albedo, metallic); }
    Vec3f get_base_reflectivity(Vec2f const& uv) const { return get_base_reflectivity(get_albedo(uv), get_metallic(uv)); }

    /**
     * @brief Applies the given set of lights to the given point on the surface with this material.
     * @param[in] lights. Set of lights in the scene.
     * @param[in] normal_direction. Direction of the normal vector in the surface point.
     * @param[in] camera_position. World-space position of the view camera.
     * @param[in] point_position. World-space position of the surface point.
     * @param[in] shadows_near_limit. Near limit for checking whether the surface point is occluded for each of the given lights.
     * @param[in] uv. Texture UV coordinate in the surface point.
     * @return The color to give to the point, as a three-dimensional vector.
     */
    Vec3f apply_lighting_in_point(std::vector<Light> const& lights, Unit_Vec3f const& normal_direction, Vec3f const& camera_position, Vec3f const& point_position, float shadows_near_limit, Vec2f const& uv) const;

  private:
    Texture m_albedo_texture;        // RGB texture specifying the diffuse surface color
    Texture m_metallic_map;          // Single-channel texture specifying the extent to which the surface is metallic (1.0) or dielectric (0.0)
    Texture m_roughness_map;         // Single-channel texture specifying the roughness of the surface
    Texture m_ambient_occlusion_map; // Single-channel texture specifying an additional shadowing factor for the surface
};
