#include "material.h"

#include <graphics/renderer/renderer.h>

Vec3f Material::apply_lighting_in_point(std::vector<Light> const& lights, Unit_Vec3f const& normal_direction, Vec3f const& camera_position, Vec3f const& point_position, float shadows_near_limit, Vec2f const& uv) const
{
    // Compute surface values
    auto const view_direction = (camera_position - point_position).normalize();
    auto const ambient_occlusion = get_ambient_occlusion(uv);
    BRDF_Input brdf_input;
    brdf_input.albedo = get_albedo(uv);
    brdf_input.roughness = get_roughness(uv);
    brdf_input.metallic = get_metallic(uv);
    brdf_input.base_reflectivity = get_base_reflectivity(brdf_input.albedo, brdf_input.metallic);

    // Add each light's contribution
    Vec3f ambient_lighting = Vec3f::zero();
    Vec3f outgoing_lighting = Vec3f::zero();
    for (auto const& light : lights)
    {
        // Apply ambient lights
        if (light.get_type() == Light_Type::ambient)
        {
            ambient_lighting += light.get_intensity() * brdf_input.albedo * ambient_occlusion;
            continue;
        }

        // Check for shadows: only compute light's contribution if it is not occluded
        auto const light_direction = light.compute_light_direction(point_position);
        auto const light_distance = light.compute_light_distance(point_position);
        if (Renderer::get_instance().intersects_any_sphere(point_position, light_direction, shadows_near_limit, light_distance))
            continue;

        // Compute relevant dot products
        auto const halfway_direction = (view_direction + light_direction).normalize();
        brdf_input.h_dot_v = (std::max)(halfway_direction.dot(view_direction), 0.0f);
        brdf_input.n_dot_h = (std::max)(normal_direction.dot(halfway_direction), 0.0f);
        brdf_input.n_dot_v = (std::max)(normal_direction.dot(view_direction), 0.0f);
        brdf_input.n_dot_l = (std::max)(normal_direction.dot(light_direction), 0.0f);

        // Compute the Cook-Torrance bidirectional reflectance distribution function and add it to the outgoing radiance
        auto const incident_lighting = light.compute_radiance(light_distance) * brdf_input.n_dot_l;
        auto const reflected_lighting = brdf_cook_torrance(brdf_input);
        outgoing_lighting += incident_lighting * reflected_lighting;
    }

    // Compute the output color, and apply tone mapping and gamma correction
    auto color = ambient_lighting + outgoing_lighting;
    color = gamma_correction(reinhard_tone_mapping(color));
    return color;
}
