#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <vector>

namespace math
{

// Forward declare the unit N-dimensional vector class.
template <class T, unsigned N> class Unit_Vec;

/**
 * @brief Defines a N-dimensional vector (2 <= N <= 4).
 * @tparam T. Numeric type of the vector.
 */
template <class T, unsigned N> class Vec
{
  public:
    virtual ~Vec() = default;
    Vec(Vec const& other) = default;
    Vec& operator=(Vec const& other) = default;

    Vec(std::array<T, N> const& components)
        : m_components{components}
    {
    }

    template <unsigned M = N>
    Vec(T const& x, typename std::enable_if_t<(M == 2), T> const& y)
        : Vec{{x, y}}
    {
    }
    template <unsigned M = N>
    Vec(typename std::enable_if_t<(M == 2), T> const& all)
        : Vec{{all, all}}
    {
    }

    template <unsigned M = N>
    Vec(T const& x, T const& y, typename std::enable_if_t<(M == 3), T> const& z)
        : Vec{{x, y, z}}
    {
    }
    template <unsigned M = N>
    Vec(typename std::enable_if_t<(M == 3), T> const& all)
        : Vec{{all, all, all}}
    {
    }

    template <unsigned M = N>
    Vec(T const& x, T const& y, T const& z, typename std::enable_if_t<(M == 4), T> const& w)
        : Vec{{x, y, z, w}}
    {
    }
    template <unsigned M = N>
    Vec(typename std::enable_if_t<(M == 4), T> const& all)
        : Vec{{all, all, all, all}}
    {
    }

    Vec operator+(T const& scalar) const
    {
        std::array<T, N> components;
        for (auto i = 0u; i < N; i++)
            components[i] = scalar + m_components[i];
        return Vec{components};
    }
    Vec operator+(Vec const& other) const
    {
        std::array<T, N> components;
        for (auto i = 0u; i < N; i++)
            components[i] = other.m_components[i] + m_components[i];
        return Vec{components};
    }
    Vec operator*(T const& scalar) const
    {
        std::array<T, N> components;
        for (auto i = 0u; i < N; i++)
            components[i] = scalar * m_components[i];
        return Vec{components};
    }
    Vec operator*(Vec const& other) const
    {
        std::array<T, N> components;
        for (auto i = 0u; i < N; i++)
            components[i] = other.m_components[i] * m_components[i];
        return Vec{components};
    }
    Vec operator/(Vec const& other) const
    {
        std::array<T, N> components;
        for (auto i = 0u; i < N; i++)
            components[i] = m_components[i] / other.m_components[i];
        return Vec{components};
    }
    Vec operator/(T const& scalar) const { return operator*(static_cast<T>(1.0f / scalar)); }
    Vec operator-(T const& scalar) const { return operator+(-scalar); }
    Vec operator-(Vec const& other) const { return operator+(-other); }
    Vec operator-() const { return operator*(static_cast<T>(-1)); }
    void operator+=(Vec const& other) { *this = operator+(other); }
    void operator*=(float const& other) { *this = operator*(other); }
    T const& operator[](unsigned int i) const { return m_components[i]; }

    /**
     * @brief Computes the dot product between this vector and another.
     * @param[in] other. The other vector with which to compute the dot product.
     * @return The dot product (a scalar value).
     */
    T dot(Vec const& other) const
    {
        T sum = 0;
        for (auto i = 0u; i < N; i++)
            sum += m_components[i] * other.m_components[i];
        return sum;
    }

    /**
     * @brief Returns the vector raised to the given power.
     * @param[in] scalar. The power at which to raise the vector.
     * @return The vector raised to the given power.
     */
    Vec<T, N> pow(T const& scalar) const
    {
        std::array<T, N> components;
        for (auto i = 0u; i < N; i++)
            components[i] = std::pow(m_components[i], scalar);
        return Vec{components};
    }

    /**
     * @brief Gets a vector with all components equal to zero.
     * @return The zero vector.
     */
    static Vec const& zero()
    {
        static Vec o{0};
        return o;
    }

    /**
     * @brief Gets a vector with all components equal to one.
     * @return The one vector.
     */
    static Vec const& one()
    {
        static Vec i{1};
        return i;
    }

    /**
     * @brief Computes the length of the vector as the square root of the dot product.
     * @return The computed length (a scalar value).
     */
    virtual T length() const { return std::sqrt(this->dot(*this)); }

    /**
     * @brief Computes the sum of the vector's components.
     * @return The computed sum (a scalar value).
     */
    virtual T sum() const
    {
        if (N == 0)
            return 0;
        auto s = m_components[0];
        for (auto i = 0; i < N; i++)
            s += m_components[i];
        return s;
    }

    /**
     * @brief Computes the average of the vector's components.
     * @return The computed average (a scalar value).
     */
    virtual T average() const { return (N > 0) ? (sum() / N) : 0; }

    /**
     * @brief Computes a normalized version of this vector.
     * @return The normalized vector (a unit vector).
     */
    virtual Unit_Vec<T, N> normalize() const { return Unit_Vec<T, N>{m_components}; }

    template <unsigned M = N> typename std::enable_if_t<(M > 0), T> x() const { return m_components[0]; }
    template <unsigned M = N> typename std::enable_if_t<(M > 1), T> y() const { return m_components[1]; }
    template <unsigned M = N> typename std::enable_if_t<(M > 2), T> z() const { return m_components[2]; }
    template <unsigned M = N> typename std::enable_if_t<(M > 3), T> w() const { return m_components[3]; }

  protected:
    std::array<T, N> m_components;
};

template <class T, unsigned N> Vec<T, N> operator+(T const& scalar, Vec<T, N> const& v) { return v + scalar; }
template <class T, unsigned N> Vec<T, N> operator*(T const& scalar, Vec<T, N> const& v) { return v * scalar; }
template <class T, unsigned N> Vec<T, N> operator/(T const& scalar, Vec<T, N> const& v) { return Vec<T, N>{scalar} / v; }
template <class T, unsigned N> Vec<T, N> operator-(T const& scalar, Vec<T, N> const& v) { return Vec<T, N>{scalar} - v; }

/**
 * @brief Defines a unit N-dimensional vector.
 * @tparam T. Numeric type of the vector.
 */
template <class T, unsigned N> class Unit_Vec : public Vec<T, N>
{
  public:
    ~Unit_Vec() = default;
    Unit_Vec(Unit_Vec const& other) = default;
    Unit_Vec& operator=(Unit_Vec const& other) = default;

    Unit_Vec(std::array<T, N> const& components)
        : Vec<T, N>{components}
    {
        T l = Vec<T, N>::length();
        if ((std::abs(l - 1.0f) > std::numeric_limits<T>::epsilon()) && (l > std::numeric_limits<T>::epsilon()))
        {
            T const& one_on_length = (1.0f / l);
            for (auto i = 0; i < N; i++)
                this->m_components[i] *= one_on_length;
        }
    }

    Unit_Vec(Vec<T, N> const& other)
        : Unit_Vec{other.normalize()}
    {
    }

    /**
     * @brief Returns the length of a unit vector, i.e. one.
     * @return One.
     */
    T length() const override { return 1.0f; }

    /**
     * @brief Returns the normalized version of this unit vector, i.e. itself.
     * @return The unit vector itself.
     */
    Unit_Vec normalize() const override { return (*this); }

    /**
     * @brief Gets a vector with the first component equal to one and all others equal to zero.
     * @return The default unit vector.
     */
    static Unit_Vec const& reference()
    {
        static Unit_Vec ref{0};
        ref.m_components[0] = 1;
        return ref;
    }
};

} // namespace math

/**
 * @brief Two-dimensional vector of unsigned integers.
 */
using Vec2u = math::Vec<unsigned int, 2u>;

/**
 * @brief Two-dimensional vector of floats.
 */
using Vec2f = math::Vec<float, 2u>;

/**
 * @brief Three-dimensional vector of floats.
 */
using Vec3f = math::Vec<float, 3u>;

/**
 * @brief Unit three-dimensional vector of floats.
 */
using Unit_Vec3f = math::Unit_Vec<float, 3u>;

namespace math
{

constexpr float z_direction_factor = -1.0f; // X right, Y top, Z forward
static Vec3f cross(Vec3f a, Vec3f b) { return z_direction_factor * Vec3f{a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()}; }

} // namespace math
