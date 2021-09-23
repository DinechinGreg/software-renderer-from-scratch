#include "renderer.h"

#include <sphere.h>
#include <vec3.h>

#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>

void renderer::setup_scene()
{
    m_spheres[0] = sphere{vec3{0.0f, -1.0f, 3.0f}, 1.0f, vec3{1.0f, 0.0f, 0.0f}};
    m_spheres[1] = sphere{vec3{2.0f, 0.0f, 4.0f}, 1.0f, vec3{0.0f, 1.0f, 0.0f}};
    m_spheres[2] = sphere{vec3{-2.0f, 0.0f, 4.0f}, 1.0f, vec3{0.0f, 0.0f, 1.0f}};
}

vec3<float> const renderer::compute_color_from_ray(vec3<float> const &origin, vec3<float> const &direction, float near,
                                                   float far)
{
    // Check if there is intersection with a sphere
    sphere const *intersected_sphere = nullptr;
    float closest = far;
    for (auto const &s : m_spheres)
    {
        std::vector<float> intersections;
        if (s.intersects_ray(origin, direction, intersections) && intersections[0] <= closest &&
            intersections[0] >= near && intersections[0] <= far)
        {
            intersected_sphere = &s;
            closest = intersections[0];
        }
    }
    // If so, return the sphere's color; otherwise, return the background color
    if (intersected_sphere != nullptr)
        return intersected_sphere->get_color();
    else
        return vec3{1.0f};
}

vec3<float> const renderer::compute_pixel_color(camera const &draw_camera, float u, float v)
{
    auto const &near = draw_camera.get_near();
    auto const &far = draw_camera.get_far();
    vec3<float> const &ray_origin{0.0f};
    vec3<float> const &ray_direction = vec3{u, v, near}.normalize();
    return compute_color_from_ray(ray_origin, ray_direction, near, far);
}

void renderer::draw_to_file(camera const &draw_camera)
{
    vec3<float> const &background_color{1.0f};
    // Create the canvas
    auto const &width = draw_camera.get_width();
    auto const &height = draw_camera.get_height();
    std::vector<vec3<float>> canvas(static_cast<size_t>(width) * static_cast<size_t>(height), background_color);
    // Compute the color of each pixel
    for (auto j = 0; j < height; j++)
    {
        for (auto i = 0; i < width; i++)
        {
            auto const &index = (height - 1 - j) * width + i;
            auto const &u = (i * 1.0f / (width - 1)) - 0.5f;
            auto const &v = (j * 1.0f / (height - 1)) - 0.5f;
            canvas[index] = compute_pixel_color(draw_camera, u, v);
        }
    }
    // Output the pixels to file
    std::ofstream ofs("./_build/output.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (auto index = 0; index < canvas.size(); index++)
    {
        ofs << static_cast<unsigned char>(std::min(1.0f, canvas[index].x) * 255)
            << static_cast<unsigned char>(std::min(1.0f, canvas[index].y) * 255)
            << static_cast<unsigned char>(std::min(1.0f, canvas[index].z) * 255);
    }
    ofs.close();
}
