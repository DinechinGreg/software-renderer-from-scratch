#pragma once

/*
 * Many of the methods in this file are based on the implementation of physically-based rendering described here :
 * https://learnopengl.com/PBR/Lighting
 */

#include <math/math.h>
#include <math/vec.h>

constexpr float dielectric_base_reflectivity = 0.04f; // Approximation of base reflectivity for dielectrics

/**
 * @brief Applies the Reinhard operator for tone mapping.
 * @param[in] color. The input color.
 * @return The tone-mapped color.
 */
static Vec3f reinhard_tone_mapping(Vec3f const& color) { return color / (1.0f + color); }

/**
 * @brief Applies gamma correction to the given color.
 * @param[in] color. The input color.
 * @param[in] gamma. The value of gamma (default: 2.2).
 * @return The gamma-corrected color.
 */
static Vec3f gamma_correction(Vec3f const& color, float gamma = 2.2f) { return color.pow(1.0f / gamma); }

/**
 * @brief Remaps roughness values in the 0-1 range to a range more suited to the BRDF methods used here.
 * @param[in] roughness_01. Roughness value between 0 and 1.
 * @return Roughness value that can be used as input to the BRDF methods used here.
 */
static float roughness_remap(float roughness_01)
{
    auto constexpr min = 0.08f;
    auto constexpr max = 1.0f;
    return roughness_01 * max + (1.0f - roughness_01) * min;
}

/**
 * @brief Computes the Fresnel-Schlick approximation of the Fresnel equation, given a base reflectivity and a surface-to-view angle.
 * @param[in] base_reflectivity. Base reflectivity of the surface.
 * @param[in] cos_theta. Cosine of the angle between the surface normal and view direction.
 * @return An approximation of the ratio of light that gets reflected over the light that gets refracted.
 */
static Vec3f fresnel_schlick(Vec3f base_reflectivity, float cos_theta) { return base_reflectivity + (1.0f - base_reflectivity) * std::pow(math::clamp(1.0f - cos_theta, 0.0f, 1.0f), 5.0f); }

/**
 * @brief Computes the Trowbridge-Reitz, or GGX (ground glass unknown), microfacet normal distribution function.
 * @param[in] n_dot_h. Dot product of the normal direction and the halfway vector between the surface normal and light direction.
 * @param[in] roughness. Surface roughness parameter, as a float between 0.0f and 1.0f.
 * @return An approximation of the relative surface area of microfacets exactly aligned to the halfway vector.
 */
static float normal_distribution_trowbridge_reitz_ggx(float n_dot_h, float roughness)
{
    auto const alpha_squared = std::pow(roughness, 4);
    auto const n_dot_h_squared = n_dot_h * n_dot_h;
    auto const numerator = alpha_squared;
    auto denominator = (n_dot_h_squared * (alpha_squared - 1.0f) + 1.0f);
    denominator = math::pi * denominator * denominator;
    return numerator / denominator;
}

/**
 * @brief Computes a combination of the GGX and Schlick-Beckmann approximation of the geometry component.
 * @param[in] n_dot_v. Dot product of the normal and view directions.
 * @param[in] k. Remapping of the surface roughness parameter.
 * @return The geometry component.
 */
static float geometry_schlick_ggx(float n_dot_v, float k)
{
    auto const numerator = n_dot_v;
    auto const denominator = n_dot_v * (1.0f - k) + k;
    return numerator / denominator;
}

/**
 * @brief Computes Smith's method for the geometry factor, taking into account both obstruction (relative to view direction) and shadowing (relative to light direction).
 * @param[in] n_dot_v. Dot product of the normal and view directions.
 * @param[in] n_dot_l. Dot product of the normal and light directions.
 * @param[in] roughness. Surface roughness parameter, as a float between 0.0f and 1.0f.
 * @return An approximation of the surface area where micro-elements of geometry overshadow each other due to obstruction and shadowing.
 */
static float geometry_smith(float n_dot_v, float n_dot_l, float roughness)
{
    auto const r = (roughness + 1.0f);
    auto const k = (r * r) / 8.0f;
    auto const geometry_obstruction = geometry_schlick_ggx(n_dot_v, k);
    auto const geometry_shadowing = geometry_schlick_ggx(n_dot_l, k);
    return geometry_obstruction * geometry_shadowing;
}
/**
 * @brief Lambertian diffuse factor.
 * @param[in] albedo. Surface color.
 * @return The diffuse part of the Cook-Torrance BRDF.
 */
static Vec3f diffuse_lambert(Vec3f const& albedo) { return albedo / math::pi; }

/**
 * @brief Cook-Torrance specular factor.
 * @param[in] normal_distribution. Result of the normal distribution function.
 * @param[in] geometry. Result of the geometry function.
 * @param[in] fresnel. Result of the Fresnel equation.
 * @param[in] n_dot_l. Dot product of the surface normal direction and light direction.
 * @param[in] n_dot_v. Dot product of the surface normal direction and view direction.
 * @return The specular part of the Cook-Torrance BRDF.
 */
static Vec3f specular_cook_torrance(float normal_distribution, float geometry, Vec3f const& fresnel, float n_dot_l, float n_dot_v)
{
    auto denominator = 4.0f * n_dot_l * n_dot_v;
    if (denominator == 0.0f)
        denominator = 0.0001f;
    return (normal_distribution * geometry * fresnel) / denominator;
}

/**
 * @brief Helper structure containing the inputs to the Cook-Torrance BRDF.
 */
struct BRDF_Input
{
    Vec3f albedo = Vec3f::zero();            // Surface color in the given point.
    float roughness = 0.0f;                  // Roughness value in the given point, as a float between 0.0 and 1.0.
    float metallic = 0.0f;                   // Metallic value in the given point, as a float between 0.0 and 1.0.
    Vec3f base_reflectivity = Vec3f::zero(); // Base reflectivity in the given point, computed as a constant for dielectrics and as albedo for metals.
    float h_dot_v = 0.0f;                    // Dot product of the halfway direction and view direction
    float n_dot_h = 0.0f;                    // Dot product of the normal direction and halfway direction
    float n_dot_v = 0.0f;                    // Dot product of the normal direction and view direction
    float n_dot_l = 0.0f;                    // Dot product of the normal direction and light direction
};

/**
 * @brief Computes the Cook-Torrance bidirectional reflectance distribution function for the given set of input parameters.
 * @param[in] input. Set of input parameters.
 * @return The result of the BRDF, as a three-dimensional vector.
 */
static Vec3f brdf_cook_torrance(BRDF_Input const& input)
{
    auto const& normal_distribution_function = normal_distribution_trowbridge_reitz_ggx;
    auto const& geometry_function = geometry_smith;
    auto const& fresnel_function = fresnel_schlick;
    auto const& diffuse_function = diffuse_lambert;
    auto const& specular_function = specular_cook_torrance;

    auto const normal_distribution = normal_distribution_function(input.n_dot_h, input.roughness);
    auto const geometry = geometry_function(input.n_dot_v, input.n_dot_l, input.roughness);
    auto const fresnel = fresnel_function(input.base_reflectivity, input.h_dot_v);

    auto const reflected_ratio = fresnel;
    auto const refracted_ratio = (1.0f - reflected_ratio) * (1.0f - input.metallic);

    return refracted_ratio * diffuse_function(input.albedo) + specular_function(normal_distribution, geometry, fresnel, input.n_dot_l, input.n_dot_v);
}
