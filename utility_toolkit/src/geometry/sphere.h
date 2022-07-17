#pragma once

#include <graphics/material.h>
#include <graphics/transform.h>
#include <math/vec.h>

#include <dll_defines.h>

#include <math/vec.h>

#include <limits>
#include <vector>

class Sphere : public Transform
{
  public:
    Sphere(Vec3f const& center = Vec3f::zero(), float radius = 0.5f, Material const& material = Material{Vec3f::zero()})
        : Transform{center}
        , m_radius{radius}
        , m_material{material}
    {
    }
    ~Sphere() = default;
    Sphere(Sphere const& other) = default;
    Sphere& operator=(Sphere const& other) = default;

    float get_radius() const { return m_radius; }
    Material const& get_material() const { return m_material; }

    /**
     * @brief Computes the texture UV value corresponding to the given normal direction of a point on the sphere.
     * @param[in] normal. Normal direction of a given point on the sphere.
     * @return The corresponding texture UV value.
     */
    static Vec2f compute_uv_from_normal(Unit_Vec3f const& normal);

  private:
    float m_radius;      // Radius of the sphere
    Material m_material; // Material used to render the sphere
};
