#pragma once

#include "primitive.h"

#include <array>

namespace geometry
{

template <unsigned N> class Polygon : public Primitive
{
  public:
    Polygon(typename std::enable_if_t<(N >= 3), std::array<Vec3f, N>> const& vertices)
        : m_vertices{vertices}
        , m_edges{vertices}
        , m_normal{Unit_Vec3f::reference()}
        , m_plane_constant{0.0f}
        , m_uv_constant{0.0f}
    {
        // Compute edges
        for (auto i = 0; i < N; i++)
            m_edges[i] = (m_vertices[(i + 1) % N] - m_vertices[i]);
        // Compute normal
        m_normal = math::cross(m_edges[0], -m_edges[N - 1]).normalize();
        // Compute constants
        m_plane_constant = -m_normal.dot(m_vertices[0]);
        m_uv_constant = math::cross(m_edges[0], m_edges[N - 1]).length();
    }
    ~Polygon() = default;
    Polygon(Polygon const& other) = default;
    Polygon& operator=(Polygon const& other) = default;

    Unit_Vec3f compute_normal_from_position_on_primitive(Vec3f const& position) const override { return m_normal; }

    Vec2f compute_uv_from_position_on_primitive(Vec3f const& position) const override
    {
        auto const u = math::cross(position - m_vertices[0], m_edges[N - 1]).length() / m_uv_constant;
        auto const v = math::cross(m_edges[0], position - m_vertices[N - 1]).length() / m_uv_constant;
        return Vec2f{u, v};
    }

    void compute_intersection_with(Ray const& ray, culling::Type culling, std::vector<float>& out_intersections) const override
    {
        // Compute the intersection with the plane within which the polygon is located
        auto const distance_along_ray = ray.compute_intersection_with_plane(m_normal, m_plane_constant, culling);
        // Return empty if there is no intersection or if the intersection point is behind the ray's origin
        if (distance_along_ray <= 0)
            return;
        // Return empty if the intersection point is not inside of the polygon
        auto const position_on_ray = ray.get_origin() + distance_along_ray * ray.get_direction();
        for (auto i = 0u; i < m_vertices.size(); i++)
        {
            auto const vertex_to_position = (position_on_ray - m_vertices[i]);
            if (m_normal.dot(cross(m_edges[i], vertex_to_position)) < 0)
                return;
        }
        // Store the distance from the ray's origin to the intersection point
        out_intersections.push_back(distance_along_ray);
    }

    Vec3f compute_center()
    {
        auto center = Vec3f::zero();
        for (auto i = 0u; i < N; i++)
            center += m_vertices[i];
        return center / N;
    }

  private:
    std::array<Vec3f, N> m_vertices; // Array of vertices
    std::array<Vec3f, N> m_edges;    // Array of edges : edge M connects vertices M and ((M+1) % N)
    Unit_Vec3f m_normal;             // Normal direction
    float m_plane_constant;          // Constant value D in the plane equation (Ax + By + Cz + D = 0) for the plane in which this polygon lies
    float m_uv_constant;             // Constant value resulting from the cross product of the two edges used to compute UV coordinate values

    static float compute_area(Vec3f const* vertices, unsigned int M)
    {
        // Add the area of the triangle formed by the last three vertices
        auto area = math::cross(vertices[M - 2] - vertices[M - 1], vertices[0] - vertices[M - 1]).length() / 2.0f;
        // If the polygon is not a triangle, add the area of the smaller polygon formed by the remaining vertices
        if (M > 3)
            area += compute_area(vertices, M - 1);
        // Return the computed area
        return area;
    }
};

using Triangle = Polygon<3>;
using Quadrilateral = Polygon<4>;

} // namespace geometry
