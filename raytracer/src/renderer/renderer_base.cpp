#include "renderer_base.h"

#include <camera.h>
#include <light.h>
#include <math/geometry.h>
#include <math/math.h>
#include <math/vec.h>
#include <sphere.h>

#include <mutex>
#include <thread>

Renderer_Base::Renderer_Base()
    : m_draw_camera{}
    , m_framebuffer_width{0}
    , m_framebuffer_height{0}
    , m_background_color{Vec3f::zero()}
    , m_spheres{}
    , m_lights{}
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
    setup_scene();
}

void Renderer_Base::release()
{
    for (auto& loading_thread : m_loading_threads)
        loading_thread.join();
}

void Renderer_Base::setup_scene()
{
    m_spheres.push_back(Sphere{Vec3f{0.0f, -1.0f, 3.0f}, 1.0f, Vec3f{1.0f, 0.0f, 0.0f}, 500.0f, 0.2f});
    m_spheres.push_back(Sphere{Vec3f{2.0f, 0.0f, 4.0f}, 1.0f, Vec3f{0.0f, 1.0f, 0.0f}, 500.0f, 0.3f});
    m_spheres.push_back(Sphere{Vec3f{-2.0f, 0.0f, 4.0f}, 1.0f, Vec3f{0.0f, 0.0f, 1.0f}, 10.0f, 0.4f});
    m_spheres.push_back(Sphere{Vec3f{0.0f, -5001.0f, 0.0f}, 5000.0f, Vec3f{1.0f, 1.0f, 0.0f}, 1000.0f, 0.5f});

    m_lights.push_back(Light{Light_Type::ambient, 0.2f});
    m_lights.push_back(Light{Light_Type::point, 0.6f, Vec3f{2.0f, 1.0f, 0.0f}});
    m_lights.push_back(Light{Light_Type::directional, 0.2f, Vec3f{-1.0f, -4.0f, -4.0f}});
}

float Renderer_Base::compute_lighting(Vec3f const& position, float distance, Unit_Vec3f const& normal, Unit_Vec3f const& view_direction, float specular_intensity) const
{
    float lighting_intensity = 0.0f;
    for (auto const& l : m_lights)
    {
        lighting_intensity += l.apply_lighting(position, distance, normal, view_direction, specular_intensity);
    }
    return lighting_intensity;
}

std::pair<Sphere const*, float> Renderer_Base::compute_closest_sphere_intersection(Vec3f const& origin, Unit_Vec3f const& direction, float near, float far) const
{
    Sphere const* intersected_sphere = nullptr;
    float closest_intersection = far;
    for (auto const& s : m_spheres)
    {
        std::vector<float> intersections;
        if (math::compute_ray_sphere_intersection(origin, direction, s.get_position(), s.get_radius(), intersections, &near, &far) && intersections[0] <= closest_intersection)
        {
            intersected_sphere = &s;
            closest_intersection = intersections[0];
        }
    }
    return {intersected_sphere, closest_intersection};
}

bool Renderer_Base::intersects_any_sphere(Vec3f const& origin, Unit_Vec3f const& direction, float near, float far, Sphere const* first_sphere_to_check) const
{
    std::vector<float> intersections;
    if (first_sphere_to_check != nullptr && math::compute_ray_sphere_intersection(origin, direction, first_sphere_to_check->get_position(), first_sphere_to_check->get_radius(), intersections, &near, &far))
        return true;
    for (auto const& s : m_spheres)
    {
        if (first_sphere_to_check != nullptr && &s == first_sphere_to_check)
            continue;
        if (math::compute_ray_sphere_intersection(origin, direction, s.get_position(), s.get_radius(), intersections, &near, &far))
            return true;
    }
    return false;
}

Vec3f const Renderer_Base::compute_color_from_ray(Vec3f const& origin, Unit_Vec3f const& direction, float near, float far, float recursion_depth) const
{
    auto const closest_sphere_intersection = compute_closest_sphere_intersection(origin, direction, near, far);
    auto const* intersected_sphere = closest_sphere_intersection.first;
    if (intersected_sphere != nullptr)
    {
        // Compute local color
        auto const& intersection_distance = closest_sphere_intersection.second;
        auto const& intersection_position = origin + intersection_distance * direction;
        auto const& intersection_normal = (intersection_position - intersected_sphere->get_position()).normalize();
        auto const& local_color = compute_lighting(intersection_position, intersection_distance, intersection_normal, -direction, intersected_sphere->get_specular_intensity()) * intersected_sphere->get_color();

        // If the object is reflective and we have not yet reached the recursion limit, send another ray
        auto const& reflective_intensity = intersected_sphere->get_reflective_intensity();
        if (recursion_depth > 0 && reflective_intensity > 0.0f)
        {
            auto const& reflected_direction = math::compute_reflected_ray(-direction, intersection_normal).normalize();
            auto const& reflected_color = compute_color_from_ray(intersection_position, reflected_direction, math::distance_epsilon(intersection_distance, 2.0f), far, recursion_depth - 1);
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
    auto const& near = m_draw_camera.get_near();
    auto const& far = m_draw_camera.get_far();
    Vec3f const& ray_origin{Vec3f::zero()};
    Unit_Vec3f const& ray_direction = Vec3f{u, v, near}.normalize();
    auto const recursion_max_depth = 1;
    return compute_color_from_ray(ray_origin, ray_direction, near, far, recursion_max_depth);
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
