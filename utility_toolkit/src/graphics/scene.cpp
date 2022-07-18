#include "scene.h"

#include <geometry/sphere.h>
#include <graphics/material.h>

#include <memory>

void Scene::setup_default_scene()
{
    auto const radius = 0.5f;
    for (auto i = 0; i < 5; i++)
    {
        for (auto j = 0; j < 4; j++)
        {
            auto const center = Vec3f{-4.0f + 2.0f * i, 2.0f * j, 0.0f};
            auto const red = (i == 0 || i == 3) ? 1.0f : 0.0f;
            auto const green = (i == 1 || i == 3 || i == 4) ? 1.0f : 0.0f;
            auto const blue = (i == 2 || i == 4) ? 1.0f : 0.0f;
            auto const albedo = Vec3f{red, green, blue};
            auto const metallic = Vec3f{(j > 0) ? 0.0f : 1.0f};
            auto const roughness = Vec3f{(j == 0 || j == 2) ? 0.5f : (j == 1) ? 0.0f : 1.0f};
            auto const material = Material{albedo, metallic, roughness};
            auto sphere = std::make_shared<geometry::Sphere>(center, radius);
            auto object = Object{sphere, material};
            m_objects.push_back(object);
        }
    }
    m_lights.push_back(Light{Light_Type::point, 30.0f, Vec3f{0.0f, 2.0f, -4.0f}});
}
