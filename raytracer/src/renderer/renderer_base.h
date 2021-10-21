#pragma once

#include <camera.h>
#include <light.h>
#include <math/vec.h>
#include <sphere.h>

#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

class renderer_base
{
  public:
    /**
     * @brief Initializes a renderer with the given data.
     * @param[in] draw_camera. The camera to use to render the scene.
     * @param[in] background_color. The background color to use for the scene.
     */
    virtual void initialize(camera const& draw_camera = camera{}, vec3f const& background_color = vec3f::zero());

    /**
     * @brief Checks whether the render loop should continue.
     * @return True if the render loop should continue, false if it should stop.
     */
    virtual bool should_continue_render_loop() const = 0;

    /**
     * @brief Draws the scene using the stored camera object.
     */
    virtual void draw_scene() = 0;

    /**
     * @brief Releases created resources.
     */
    virtual void release();

    /**
     * @brief Computes the lighting intensity in a given position, based on the lights in the scene.
     * @param[in] position. Position of the point in which to compute lighting, resulting from an intersection between a ray and the scene's geometry.
     * @param[in] distance. Distance separating the point from the ray's origin, used to have a notion of the precision of the intersection position.
     * @param[in] normal. Normal direction of the surface in the given point, as a unit vector.
     * @param[in] view_direction. Direction from the point to the origin of the ray, as a unit vector.
     * @param[in] specular_intensity. Intensity factor to use for the computation of the specular component.
     * @return The lighting intensity in the given point.
     */
    float compute_lighting(vec3f const& position, float distance, unit_vec3f const& normal, unit_vec3f const& view_direction, float specular_intensity) const;

    /**
     * @brief Computes the closest intersection between the ray and an element of the scene's geometry.
     * @param[in] origin. Origin of the ray.
     * @param[in] direction. Direction of the ray, as a unit vector.
     * @param[in] near. Near intersection distance, at which to start looking for intersections.
     * @param[in] far. Far intersection distance, at which to stop looking for intersections.
     * @return A pair of values, containing a pointer to the intersected element and the distance separating the intersection from the ray's origin.
     */
    std::pair<sphere const*, float> compute_closest_sphere_intersection(vec3f const& origin, unit_vec3f const& direction, float near, float far) const;

    /**
     * @brief Checks whether the given ray intersects any element of the scene's geometry.
     * @param[in] origin. Origin of the ray.
     * @param[in] direction. Direction of the ray, as a unit vector.
     * @param[in] near. Near intersection distance, at which to start looking for intersections.
     * @param[in] far. Far intersection distance, at which to stop looking for intersections.
     * @param[in] first_sphere_to_check. (Optional) Element of geometry which is expected to intersect with the ray, to check first.
     * @return True if the ray intersects with an element, false otherwise.
     */
    bool intersects_any_sphere(vec3f const& origin, unit_vec3f const& direction, float near, float far, sphere const* first_sphere_to_check = nullptr) const;

    /**
     * @brief Computes the color obtained by intersecting the given ray with the scene's geometry.
     * @param[in] origin. Origin of the ray.
     * @param[in] direction. Direction of the ray, as a unit vector.
     * @param[in] near. Near intersection distance, at which to start looking for intersections.
     * @param[in] far. Far intersection distance, at which to stop looking for intersections.
     * @param[in] recursion_depth. Number of times we reflect the ray off the geometry to look for reflected colors.
     * @return The computed color.
     */
    vec3f const compute_color_from_ray(vec3f const& origin, unit_vec3f const& direction, float near, float far, float recursion_depth) const;

    /**
     * @brief Computes the color in the given pixel.
     * @param[in] u. Horizontal pixel identifier, as a value between zero and one.
     * @param[in] v. Vertical pixel identifier, as a value between zero and one.
     * @return The computed color.
     */
    vec3f const compute_pixel_color(float u, float v) const;

  protected:
    renderer_base();
    virtual ~renderer_base() = default;
    renderer_base(renderer_base const& other) = delete;
    renderer_base& operator=(renderer_base const& other) = delete;

    /**
     * @brief Sets up lights and geometry in the scene.
     */
    void setup_scene();

    /**
     * @brief Computes and stores the colors of the next unhandled row of the framebuffer.
     * This method is made to be called asynchronously using the group of member threads.
     */
    void compute_pixel_colors_for_next_row();

    /**
     * @brief Launches the thread group that comptes the pixel colors.
     */
    void launch_pixel_loading_threads();

    camera m_draw_camera;                                        // Camera to use to draw the scene
    int m_framebuffer_width;                                     // Width of the framebuffer
    int m_framebuffer_height;                                    // Height of the framebuffer
    vec3f m_background_color;                                    // Background color of the framebuffer
    std::vector<sphere> m_spheres;                               // List of spheres that composes the scene's geometry
    std::vector<light> m_lights;                                 // List of lights that composes the scene's lighting
    std::vector<std::thread> m_loading_threads;                  // List of threads to use to compute the output colors asynchronously
    std::mutex m_thread_guard;                                   // Thread guard used to prevent concurrent writing to the stored output values
    volatile std::atomic<int> m_last_loaded_row;                 // Index of the last row that has been handled by a loading thread
    std::unordered_map<int, std::vector<float>> m_loaded_pixels; // Maps the index of each loaded row to the pixels that have been loaded for this row
};
