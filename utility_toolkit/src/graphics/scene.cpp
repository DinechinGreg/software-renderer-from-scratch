#include "scene.h"

#include <geometry/polygon.h>
#include <geometry/sphere.h>
#include <graphics/material.h>

#include <memory>

void Scene::setup_default_scene()
{
    // Create materials
    auto const white_roughness = 0.3f;
    auto const other_roughness = 0.6f;
    auto const white_material = Material{Vec3f::one(), Vec3f::zero(), white_roughness * Vec3f::one()};
    auto const metal_material = Material{Vec3f::one(), Vec3f::one(), white_roughness * Vec3f::zero()};
    auto const red_material = Material{Vec3f{1, 0, 0}, Vec3f::zero(), other_roughness * Vec3f::one()};
    auto const green_material = Material{Vec3f{0, 1, 0}, Vec3f::zero(), other_roughness * Vec3f::one()};
    auto const blue_material = Material{Vec3f{0, 0, 1}, Vec3f::zero(), other_roughness * Vec3f::one()};
    auto const cyan_material = Material{Vec3f{0, 1, 1}, Vec3f::zero(), Vec3f::one()};

    // Setup Cornell Box cube
    auto const cube_size = 1.0f;
    auto const cube_center = Vec3f{0.0f, 0.0f, 3.0f};
    auto const cube_back_bottom_left = cube_center + Vec3f{-cube_size, -cube_size, cube_size};
    auto const cube_back_bottom_right = cube_center + Vec3f{cube_size, -cube_size, cube_size};
    auto const cube_back_top_left = cube_center + Vec3f{-cube_size, cube_size, cube_size};
    auto const cube_back_top_right = cube_center + Vec3f{cube_size, cube_size, cube_size};
    auto const cube_front_bottom_left = cube_center + Vec3f{-cube_size, -cube_size, -cube_size};
    auto const cube_front_bottom_right = cube_center + Vec3f{cube_size, -cube_size, -cube_size};
    auto const cube_front_top_left = cube_center + Vec3f{-cube_size, cube_size, -cube_size};
    auto const cube_front_top_right = cube_center + Vec3f{cube_size, cube_size, -cube_size};
    auto const back_wall = std::make_shared<geometry::Quadrilateral>(std::array<Vec3f, 4>{cube_back_bottom_left, cube_back_bottom_right, cube_back_top_right, cube_back_top_left});
    auto const front_wall = std::make_shared<geometry::Quadrilateral>(std::array<Vec3f, 4>{cube_front_bottom_right, cube_front_bottom_left, cube_front_top_left, cube_front_top_right});
    auto const left_wall = std::make_shared<geometry::Quadrilateral>(std::array<Vec3f, 4>{cube_front_bottom_left, cube_back_bottom_left, cube_back_top_left, cube_front_top_left});
    auto const right_wall = std::make_shared<geometry::Quadrilateral>(std::array<Vec3f, 4>{cube_back_bottom_right, cube_front_bottom_right, cube_front_top_right, cube_back_top_right});
    auto const top_wall = std::make_shared<geometry::Quadrilateral>(std::array<Vec3f, 4>{cube_back_top_left, cube_back_top_right, cube_front_top_right, cube_front_top_left});
    auto const bottom_wall = std::make_shared<geometry::Quadrilateral>(std::array<Vec3f, 4>{cube_front_bottom_left, cube_front_bottom_right, cube_back_bottom_right, cube_back_bottom_left});
    m_objects.push_back(Object{"Back wall", back_wall, white_material});
    m_objects.push_back(Object{"Front wall", front_wall, cyan_material});
    m_objects.push_back(Object{"Left wall", left_wall, red_material});
    m_objects.push_back(Object{"Right wall", right_wall, blue_material});
    m_objects.push_back(Object{"Top wall", top_wall, white_material});
    m_objects.push_back(Object{"Bottom wall", bottom_wall, white_material});

    // Setup Cornell Box spheres
    auto const sphere_base_position = bottom_wall->compute_center();
    auto const sphere_position_offset = 0.4f * cube_size;
    auto const sphere_radius = 0.5f * cube_size;
    auto const left_sphere = std::make_shared<geometry::Sphere>(sphere_base_position + Vec3f{-sphere_position_offset, sphere_radius, sphere_position_offset}, sphere_radius);
    auto const right_sphere = std::make_shared<geometry::Sphere>(sphere_base_position + Vec3f{sphere_position_offset, sphere_radius, -sphere_position_offset}, sphere_radius);
    m_objects.push_back(Object{"Left sphere", left_sphere, metal_material});
    m_objects.push_back(Object{"Right sphere", right_sphere, green_material});

    // Setup lights
    auto const lights_position_offset_from_wall = 0.1f * cube_size;
    auto const lights_position_offset_z = -5.0f * lights_position_offset_from_wall;
    auto const light_intensity = cube_size;
    auto const top_light = Light{Light_Type::point, light_intensity, top_wall->compute_center() + Vec3f{0.0f, -lights_position_offset_from_wall, 0.0f}};
    auto const left_light = Light{Light_Type::point, light_intensity, left_wall->compute_center() + Vec3f{lights_position_offset_from_wall, 0.0f, lights_position_offset_z}};
    auto const right_light = Light{Light_Type::point, light_intensity, right_wall->compute_center() + Vec3f{-lights_position_offset_from_wall, 0.0f, lights_position_offset_z}};
    m_lights.push_back(top_light);
    m_lights.push_back(left_light);
    m_lights.push_back(right_light);
}
