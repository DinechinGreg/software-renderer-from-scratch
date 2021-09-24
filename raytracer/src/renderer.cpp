#include "renderer.h"

#include <light.h>
#include <sphere.h>
#include <unit_vec3.h>
#include <vec3.h>

#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>

renderer::renderer(vec3<float> const& background_color)
    : m_background_color{background_color}
{
}

void renderer::setup_scene()
{
    m_spheres.push_back(sphere{vec3{0.0f, -1.0f, 3.0f}, 1.0f, vec3{1.0f, 0.0f, 0.0f}, 500.0f, 0.2f});
    m_spheres.push_back(sphere{vec3{2.0f, 0.0f, 4.0f}, 1.0f, vec3{0.0f, 1.0f, 0.0f}, 500.0f, 0.3f});
    m_spheres.push_back(sphere{vec3{-2.0f, 0.0f, 4.0f}, 1.0f, vec3{0.0f, 0.0f, 1.0f}, 10.0f, 0.4f});
    m_spheres.push_back(sphere{vec3{0.0f, -5001.0f, 0.0f}, 5000.0f, vec3{1.0f, 1.0f, 0.0f}, 1000.0f, 0.5f});

    m_lights.push_back(light{light_type::ambient, 0.2f});
    m_lights.push_back(light{light_type::point, 0.6f, vec3{2.0f, 1.0f, 0.0f}});
    m_lights.push_back(light{light_type::directional, 0.2f, vec3{-1.0f, -4.0f, -4.0f}});
}

float renderer::compute_lighting(vec3<float> const& position, vec3<float> const& normal, vec3<float> const& view_direction, float specular_intensity) const
{
    float lighting_intensity = 0.0f;
    for (auto const& l : m_lights)
    {
        lighting_intensity += l.apply_lighting(position, normal, view_direction, specular_intensity);
    }
    return lighting_intensity;
}

std::pair<sphere const*, float> renderer::compute_closest_sphere_intersection(vec3<float> const& origin, unit_vec3<float> const& direction, float near, float far) const
{
    sphere const* intersected_sphere = nullptr;
    float closest_intersection = far;
    for (auto const& s : m_spheres)
    {
        std::vector<float> intersections;
        if (s.intersects_ray(origin, direction, intersections) && intersections[0] <= closest_intersection && intersections[0] >= near && intersections[0] <= far)
        {
            intersected_sphere = &s;
            closest_intersection = intersections[0];
        }
    }
    return {intersected_sphere, closest_intersection};
}

vec3<float> const renderer::compute_color_from_ray(vec3<float> const& origin, unit_vec3<float> const& direction, float near, float far, float recursion_depth) const
{
    auto const closest_sphere_intersection = compute_closest_sphere_intersection(origin, direction, near, far);
    auto const* intersected_sphere = closest_sphere_intersection.first;
    if (intersected_sphere != nullptr)
    {
        // Compute local color
        auto const& intersection_position = origin + closest_sphere_intersection.second * direction;
        auto const& intersection_normal = (intersection_position - intersected_sphere->get_position()).normalize();
        auto const& local_color = compute_lighting(intersection_position, intersection_normal, -direction, intersected_sphere->get_specular_intensity()) * intersected_sphere->get_color();

        // If the object is reflective and we have not yet reached the recursion limit, send another ray
        auto const& reflective_intensity = intersected_sphere->get_reflective_intensity();
        if (recursion_depth > 0 && reflective_intensity > 0.0f)
        {
            auto const& reflected_direction = renderer::get_instance().reflect_ray(-direction, intersection_normal).normalize();
            auto const& reflected_color = compute_color_from_ray(intersection_position, reflected_direction, std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::infinity(), recursion_depth - 1);
            return (1.0f - reflective_intensity) * local_color + reflective_intensity * reflected_color;
        }
        else
        {
            return local_color;
        }
    }
    else
    {
        return m_background_color;
    }
}

vec3<float> const renderer::compute_pixel_color(camera const& draw_camera, float u, float v) const
{
    auto const& near = draw_camera.get_near();
    auto const& far = draw_camera.get_far();
    vec3<float> const& ray_origin{vec3<float>::zero()};
    vec3<float> const& ray_direction = vec3{u, v, near};
    auto const recursion_max_depth = 3;
    return compute_color_from_ray(ray_origin, ray_direction, near, far, recursion_max_depth);
}

void renderer::draw_to_file(camera const& draw_camera) const
{
    // Create the canvas
    auto const& width = draw_camera.get_width();
    auto const& height = draw_camera.get_height();
    std::vector<vec3<float>> canvas(static_cast<size_t>(width) * static_cast<size_t>(height), m_background_color);
    // Compute the color of each pixel
    for (auto j = 0; j < height; j++)
    {
        for (auto i = 0; i < width; i++)
        {
            auto const& index = (height - 1 - j) * width + i;
            auto const& u = (i * 1.0f / (width - 1)) - 0.5f;
            auto const& v = (j * 1.0f / (height - 1)) - 0.5f;
            canvas[index] = compute_pixel_color(draw_camera, u, v);
        }
    }
    // Output the pixels to file
    std::ofstream ofs("./_build/output.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (auto index = 0; index < canvas.size(); index++)
    {
        ofs << static_cast<unsigned char>(std::min(1.0f, canvas[index].x) * 255) << static_cast<unsigned char>(std::min(1.0f, canvas[index].y) * 255) << static_cast<unsigned char>(std::min(1.0f, canvas[index].z) * 255);
    }
    ofs.close();
}

vec3<float> const renderer::reflect_ray(vec3<float> const& ray, unit_vec3<float> const& around, float const* precomputed_dot) const
{
    float dot = (precomputed_dot == nullptr) ? ray.dot(around) : *precomputed_dot;
    return 2.0f * dot * around - ray;
}
