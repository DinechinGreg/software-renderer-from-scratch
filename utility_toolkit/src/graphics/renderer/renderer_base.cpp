#include "renderer_base.h"

#include <graphics/camera.h>
#include <graphics/light.h>
#include <graphics/material.h>
#include <graphics/sphere.h>
#include <math/geometry.h>
#include <math/math.h>
#include <math/vec.h>

#include <mutex>
#include <thread>

Renderer_Base::Renderer_Base()
    : m_draw_camera{}
    , m_framebuffer_width{0}
    , m_framebuffer_height{0}
    , m_background_color{Vec3f::zero()}
    , m_scene{}
    , m_loading_threads{}
    , m_thread_guard{}
    , m_last_loaded_row{0}
    , m_loaded_pixels{}
{
}

void Renderer_Base::initialize(Camera const& draw_camera, Vec3f const& background_color)
{
    m_draw_camera = draw_camera;
    m_background_color = background_color;
    m_framebuffer_width = draw_camera.get_width();
    m_framebuffer_height = draw_camera.get_height();
    m_scene.setup_default_scene();
}

void Renderer_Base::release()
{
    for (auto& loading_thread : m_loading_threads)
        loading_thread.join();
}

std::pair<Sphere const*, float> Renderer_Base::compute_closest_sphere_intersection(Vec3f const& origin, Unit_Vec3f const& direction, float near_limit, float far_limit) const
{
    Sphere const* intersected_sphere = nullptr;
    float closest_intersection = far_limit;
    for (auto const& s : m_scene.get_objects())
    {
        std::vector<float> intersections;
        if (math::compute_ray_sphere_intersection(origin, direction, s.get_position(), s.get_radius(), intersections, &near_limit, &far_limit) && intersections[0] <= closest_intersection)
        {
            intersected_sphere = &s;
            closest_intersection = intersections[0];
        }
    }
    return {intersected_sphere, closest_intersection};
}

bool Renderer_Base::intersects_any_sphere(Vec3f const& origin, Unit_Vec3f const& direction, float near_limit, float far_limit, Sphere const* first_sphere_to_check) const
{
    std::vector<float> intersections;
    if (first_sphere_to_check != nullptr && math::compute_ray_sphere_intersection(origin, direction, first_sphere_to_check->get_position(), first_sphere_to_check->get_radius(), intersections, &near_limit, &far_limit))
        return true;
    for (auto const& s : m_scene.get_objects())
    {
        if (first_sphere_to_check != nullptr && &s == first_sphere_to_check)
            continue;
        if (math::compute_ray_sphere_intersection(origin, direction, s.get_position(), s.get_radius(), intersections, &near_limit, &far_limit))
            return true;
    }
    return false;
}

Vec3f const Renderer_Base::compute_color_from_ray(Vec3f const& origin, Unit_Vec3f const& direction, float near_limit, float far_limit, float recursion_depth) const
{
    auto const closest_sphere_intersection = compute_closest_sphere_intersection(origin, direction, near_limit, far_limit);
    auto const* intersected_sphere = closest_sphere_intersection.first;
    if (intersected_sphere != nullptr)
    {
        // Compute local color
        auto const& sphere_material = intersected_sphere->get_material();
        auto const& intersection_distance = closest_sphere_intersection.second;
        auto const shadows_near_limit = math::distance_epsilon(intersection_distance, 1.0f, 1e-2f);
        auto const intersection_position = origin + intersection_distance * direction;
        auto const intersection_normal = (intersection_position - intersected_sphere->get_position()).normalize();
        auto const intersection_uv = intersected_sphere->compute_uv_from_normal(intersection_normal);
        auto const local_color = sphere_material.apply_lighting_in_point(m_scene.get_lights(), intersection_normal, m_draw_camera.get_position(), intersection_position, shadows_near_limit, intersection_uv);

        // If the object is reflective and we have not yet reached the recursion limit, send another ray
        // TODO : make this depend on the material's properties, as the reflective intensity is set arbitrarily for now
        if (recursion_depth > 0)
        {
            auto const reflective_intensity = 0.2f;
            auto const& reflected_direction = math::compute_reflected_ray(-direction, intersection_normal).normalize();
            auto const& reflected_color = compute_color_from_ray(intersection_position, reflected_direction, math::distance_epsilon(intersection_distance, 2.0f), far_limit, recursion_depth - 1);
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

Vec3f const Renderer_Base::compute_pixel_color(float u, float v) const
{
    auto const& near_limit = m_draw_camera.get_near();
    auto const& far_limit = m_draw_camera.get_far();
    Vec3f const& ray_origin{m_draw_camera.get_position()};
    Unit_Vec3f const& ray_direction = Vec3f{u, v, near_limit}.normalize();
    auto const recursion_max_depth = 1;
    return compute_color_from_ray(ray_origin, ray_direction, near_limit, far_limit, recursion_max_depth);
}

void Renderer_Base::launch_pixel_loading_threads()
{
    // Get the number of CPU cores
    auto const core_count = std::thread::hardware_concurrency();
    // Launch one thread per core
    for (auto it = 0u; it < core_count; it++)
        m_loading_threads.push_back(std::thread(&Renderer_Base::compute_pixel_colors_for_next_row, this));
}

void Renderer_Base::compute_pixel_colors_for_next_row()
{
    // Keep looking for a next row of pixels to compute, until there are no more, in which case return
    while (true)
    {
        // Get the index of the next unhandled row, and return if it is beyond the framebuffer height
        auto j = m_last_loaded_row++;
        if (j > m_framebuffer_height)
            return;
        // Compute values for each pixel in the row, and store them in the member storage variable
        for (auto i = 0; i < m_framebuffer_width; i++)
        {
            auto const index = (j * m_framebuffer_width + i);
            auto const u = (i * 1.0f / (m_framebuffer_width - 1)) - 0.5f;
            auto const v = (j * 1.0f / (m_framebuffer_height - 1)) - 0.5f;
            auto const pixel_color = compute_pixel_color(u, v);
            // Move the computed pixel values into the member storage variable in a thread-safe way
            m_thread_guard.lock();
            m_loaded_pixels.insert(std::make_pair(index, pixel_color));
            m_thread_guard.unlock();
        }
    }
}
