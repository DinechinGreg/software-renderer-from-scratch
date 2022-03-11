#pragma once

#include <graphics/camera.h>
#include <graphics/light.h>
#include <graphics/sphere.h>
#include <math/vec.h>

#include <dll_defines.h>

#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

class Renderer_Base
{
  public:
    /**
     * @brief Initializes a renderer with the given data.
     * @param[in] draw_camera. The camera to use to render the scene.
     * @param[in] background_color. The background color to use for the scene.
     */
    DECLSPECIFIER virtual void initialize(Camera const& draw_camera = Camera{}, Vec3f const& background_color = Vec3f::zero());

    /**
     * @brief Checks whether the render loop should continue.
     * @return True if the render loop should continue, false if it should stop.
     */
    DECLSPECIFIER virtual bool should_continue_render_loop() const = 0;

    /**
     * @brief Draws the scene using the stored camera object.
     */
    DECLSPECIFIER virtual void draw_scene() = 0;

    /**
     * @brief Releases created resources.
     */
    DECLSPECIFIER virtual void release();

    /**
     * @brief Computes the closest intersection between the ray and an element of the scene's geometry.
     * @param[in] origin. Origin of the ray.
     * @param[in] direction. Direction of the ray, as a unit vector.
     * @param[in] near_limit. Near intersection distance, at which to start looking for intersections.
     * @param[in] far_limit. Far intersection distance, at which to stop looking for intersections.
     * @return A pair of values, containing a pointer to the intersected element and the distance separating the intersection from the ray's origin.
     */
    DECLSPECIFIER std::pair<Sphere const*, float> compute_closest_sphere_intersection(Vec3f const& origin, Unit_Vec3f const& direction, float near_limit, float far_limit) const;

    /**
     * @brief Checks whether the given ray intersects any element of the scene's geometry.
     * @param[in] origin. Origin of the ray.
     * @param[in] direction. Direction of the ray, as a unit vector.
     * @param[in] near_limit. Near intersection distance, at which to start looking for intersections.
     * @param[in] far_limit. Far intersection distance, at which to stop looking for intersections.
     * @param[in] first_sphere_to_check. (Optional) Element of geometry which is expected to intersect with the ray, to check first.
     * @return True if the ray intersects with an element, false otherwise.
     */
    DECLSPECIFIER bool intersects_any_sphere(Vec3f const& origin, Unit_Vec3f const& direction, float near_limit, float far_limit, Sphere const* first_sphere_to_check = nullptr) const;

    /**
     * @brief Computes the color obtained by intersecting the given ray with the scene's geometry.
     * @param[in] origin. Origin of the ray.
     * @param[in] direction. Direction of the ray, as a unit vector.
     * @param[in] near_limit. Near intersection distance, at which to start looking for intersections.
     * @param[in] far_limit. Far intersection distance, at which to stop looking for intersections.
     * @param[in] recursion_depth. Number of times we reflect the ray off the geometry to look for reflected colors.
     * @return The computed color.
     */
    DECLSPECIFIER Vec3f const compute_color_from_ray(Vec3f const& origin, Unit_Vec3f const& direction, float near_limit, float far_limit, float recursion_depth) const;

    /**
     * @brief Computes the color in the given pixel.
     * @param[in] u. Horizontal pixel identifier, as a value between zero and one.
     * @param[in] v. Vertical pixel identifier, as a value between zero and one.
     * @return The computed color.
     */
    DECLSPECIFIER Vec3f const compute_pixel_color(float u, float v) const;

  protected:
    DECLSPECIFIER Renderer_Base();
    DECLSPECIFIER virtual ~Renderer_Base() = default;
    DECLSPECIFIER Renderer_Base(Renderer_Base const& other) = delete;
    DECLSPECIFIER Renderer_Base& operator=(Renderer_Base const& other) = delete;

    /**
     * @brief Sets up lights and geometry in the scene.
     */
    DECLSPECIFIER void setup_scene();

    /**
     * @brief Computes and stores the colors of the next unhandled row of the framebuffer.
     * This method is made to be called asynchronously using the group of member threads.
     */
    DECLSPECIFIER void compute_pixel_colors_for_next_row();

    /**
     * @brief Launches the thread group that comptes the pixel colors.
     */
    DECLSPECIFIER void launch_pixel_loading_threads();

    Camera m_draw_camera;                           // Camera to use to draw the scene
    int m_framebuffer_width;                        // Width of the framebuffer
    int m_framebuffer_height;                       // Height of the framebuffer
    Vec3f m_background_color;                       // Background color of the framebuffer
    std::vector<Sphere> m_spheres;                  // List of spheres that composes the scene's geometry
    std::vector<Light> m_lights;                    // List of lights that composes the scene's lighting
    std::vector<std::thread> m_loading_threads;     // List of threads to use to compute the output colors asynchronously
    std::mutex m_thread_guard;                      // Thread guard used to prevent concurrent writing to the stored output values
    volatile std::atomic<int> m_last_loaded_row;    // Index of the last row that has been handled by a loading thread
    std::unordered_map<int, Vec3f> m_loaded_pixels; // Maps each loaded pixel's index in the canvas to its R,G,B values
};
