#include "renderer_base.h"

#include <geometry/ray.h>
#include <graphics/camera.h>
#include <graphics/light.h>
#include <graphics/material.h>
#include <graphics/object.h>
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
    , m_culling_type{culling::Type::BackFace}
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

std::pair<Object const*, float> Renderer_Base::compute_closest_intersection_with_scene(geometry::Ray const& ray, float near_limit, float far_limit) const
{
    std::vector<float> intersections;
    Object const* intersected_object = nullptr;
    float closest_intersection = far_limit;
    for (auto const& object : m_scene.get_objects())
    {
        object.get_primitive().compute_intersection_with(ray, near_limit, far_limit, m_culling_type, intersections);
        if (intersections.size() > 0 && intersections[0] <= closest_intersection)
        {
            intersected_object = &object;
            closest_intersection = intersections[0];
        }
    }
    return {intersected_object, closest_intersection};
}

bool Renderer_Base::intersects_any_object(geometry::Ray const& ray, float near_limit, float far_limit, bool invert_culling, Object const* first_element_to_check) const
{
    auto const culling_type = (invert_culling ? culling::opposite(m_culling_type) : m_culling_type);
    std::vector<float> intersections;
    if (first_element_to_check != nullptr)
    {
        first_element_to_check->get_primitive().compute_intersection_with(ray, near_limit, far_limit, culling_type, intersections);
        if (intersections.size() > 0)
            return true;
    }
    for (auto const& object : m_scene.get_objects())
    {
        if (first_element_to_check != nullptr && &object == first_element_to_check)
            continue;
        object.get_primitive().compute_intersection_with(ray, near_limit, far_limit, culling_type, intersections);
        if (intersections.size() > 0)
            return true;
    }
    return false;
}

Vec3f const Renderer_Base::compute_color_from_ray(geometry::Ray const& ray, float near_limit, float far_limit, float recursion_depth) const
{
    auto const closest_intersection = compute_closest_intersection_with_scene(ray, near_limit, far_limit);
    auto const* intersected_object = closest_intersection.first;
    if (intersected_object != nullptr)
    {
        // Compute local color
        auto const& object_material = intersected_object->get_material();
        auto const& object_primitive = intersected_object->get_primitive();
        auto const& intersection_distance = closest_intersection.second;
        auto const shadows_near_limit = math::distance_epsilon(intersection_distance, 1.0f, 1e-2f);
        auto const& ray_direction = ray.get_direction();
        auto const& ray_origin = ray.get_origin();
        auto const intersection_position = ray_origin + intersection_distance * ray_direction;
        auto const intersection_normal = object_primitive.compute_normal_from_position_on_primitive(intersection_position);
        auto const intersection_uv = object_primitive.compute_uv_from_position_on_primitive(intersection_position);
        auto const local_color = object_material.apply_lighting_in_point(m_scene.get_lights(), intersection_normal, m_draw_camera.get_position(), intersection_position, shadows_near_limit, intersection_uv);

        // If the object is reflective and we have not yet reached the recursion limit, send another ray
        // TODO : make this depend on the material's properties, as the reflective intensity is set arbitrarily for now
        if (recursion_depth > 0)
        {
            auto const reflective_intensity = std::powf((1.0f - object_material.get_roughness(intersection_uv)), 5.0f);
            auto const& reflected_ray = geometry::Ray{intersection_position, -ray_direction}.reflect(intersection_normal);
            auto const& reflected_color = compute_color_from_ray(reflected_ray, math::distance_epsilon(intersection_distance, 2.0f), far_limit, recursion_depth - 1);
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
    geometry::Ray ray{ray_origin, ray_direction};
    auto const recursion_max_depth = 1;
    return compute_color_from_ray(ray, near_limit, far_limit, recursion_max_depth);
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
