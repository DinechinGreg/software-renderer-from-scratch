#pragma once

#include "primitive.h"

#include <math/vec.h>

namespace geometry
{

class Sphere : public Primitive
{
  public:
    Sphere(Vec3f const& origin = Vec3f::zero(), float radius = 0.5f)
        : m_origin{origin}
        , m_radius{radius}
    {
    }
    ~Sphere() = default;
    Sphere(Sphere const& other) = default;
    Sphere& operator=(Sphere const& other) = default;

    Vec3f const& get_origin() const { return m_origin; }
    float get_radius() const { return m_radius; }

    Unit_Vec3f compute_normal_from_position_on_primitive(Vec3f const& position) const override;
    Vec2f compute_uv_from_position_on_primitive(Vec3f const& position) const override;
    void compute_intersection_with(Ray const& ray, culling::Type culling, std::vector<float>& out_intersections) const override;

  private:
    Vec3f m_origin; // Origin of the sphere
    float m_radius; // Radius of the sphere
};

} // namespace geometry
