#pragma once

#include "material.h"
#include "transform.h"

#include <geometry/primitive.h>

#include <memory>

class Object : Transform
{
  public:
    Object(std::shared_ptr<geometry::Primitive> const& primitive, Material const& material = Material{Vec3f::zero()})
        : m_primitive{primitive}
        , m_material{material}
    {
    }
    ~Object() = default;
    Object(Object const& other) = default;
    Object& operator=(Object const& other) = default;

    geometry::Primitive const& get_primitive() const { return *m_primitive; }
    Material const& get_material() const { return m_material; }

  private:
    std::shared_ptr<geometry::Primitive> m_primitive; // Geometry of the object
    Material m_material;                              // Material used to render the object
};
