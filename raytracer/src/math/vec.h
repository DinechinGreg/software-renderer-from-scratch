#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <vector>

// Forward declare the unit N-dimensional vector class.
template <class T, unsigned N> class unit_vec;

/**
 * @brief Defines a N-dimensional vector (2 <= N <= 4).
 * @tparam T. Numeric type of the vector.
 */
template <class T, unsigned N> class vec
{
  public:
    virtual ~vec() = default;
    vec(vec const& other) = default;
    vec& operator=(vec const& other) = default;

    vec(std::array<T, N> const& components)
        : m_components{components}
    {
    }

    template <unsigned M = N>
    vec(T const& x, typename std::enable_if_t<(M == 2), T> const& y)
        : vec{{x, y}}
    {
    }
    template <unsigned M = N>
    vec(typename std::enable_if_t<(M == 2), T> const& all)
        : vec{{all, all}}
    {
    }

    template <unsigned M = N>
    vec(T const& x, T const& y, typename std::enable_if_t<(M == 3), T> const& z)
        : vec{{x, y, z}}
    {
    }
    template <unsigned M = N>
    vec(typename std::enable_if_t<(M == 3), T> const& all)
        : vec{{all, all, all}}
    {
    }

    template <unsigned M = N>
    vec(T const& x, T const& y, T const& z, typename std::enable_if_t<(M == 4), T> const& w)
        : vec{{x, y, z, w}}
    {
    }
    template <unsigned M = N>
    vec(typename std::enable_if_t<(M == 4), T> const& all)
        : vec{{all, all, all, all}}
    {
    }

    vec operator+(vec const& other) const
    {
        std::array<T, N> components;
        for (auto it = 0u; it < N; it++)
            components[it] = m_components[it] + other.m_components[it];
        return vec{components};
    }
    vec operator*(T const& scalar) const
    {
        std::array<T, N> components;
        for (auto it = 0u; it < N; it++)
            components[it] = scalar * m_components[it];
        return vec{components};
    }
    vec operator-(vec const& other) const { return operator+(-other); }
    vec operator-() const { return (*this) * static_cast<T>(-1); }

    /**
     * @brief Computes the dot product between this vector and another.
     * @param other. The other vector with which to compute the dot product.
     * @return The dot product (a scalar value).
     */
    T dot(vec const& other) const
    {
        T sum = 0;
        for (auto it = 0u; it < N; it++)
            sum += m_components[it] * other.m_components[it];
        return sum;
    }

    /**
     * @brief Gets a vector with all components equal to zero.
     * @return The zero vector.
     */
    static vec const& zero()
    {
        static vec o{0};
        return o;
    }

    /**
     * @brief Gets a vector with all components equal to one.
     * @return The one vector.
     */
    static vec const& one()
    {
        static vec i{1};
        return i;
    }

    /**
     * @brief Computes the length of the vector as the square root of the dot product.
     * @return The computed length (a scalar value).
     */
    virtual T length() const { return std::sqrt(this->dot(*this)); }

    /**
     * @brief Computes a normalized version of this vector.
     * @return The normalized vector (a unit vector).
     */
    virtual unit_vec<T, N> normalize() const;

    template <unsigned M = N> typename std::enable_if_t<(M > 0), T> x() { return m_components[0]; }
    template <unsigned M = N> typename std::enable_if_t<(M > 1), T> y() { return m_components[1]; }
    template <unsigned M = N> typename std::enable_if_t<(M > 2), T> z() { return m_components[2]; }
    template <unsigned M = N> typename std::enable_if_t<(M > 3), T> w() { return m_components[3]; }

  protected:
    std::array<T, N> m_components;
};

template <class T, unsigned N> vec<T, N> operator*(T const& scalar, vec<T, N> const& v) { return v * scalar; }

/**
 * @brief Defines a unit N-dimensional vector.
 * @tparam T. Numeric type of the vector.
 */
template <class T, unsigned N> class unit_vec : public vec<T, N>
{
  public:
    ~unit_vec() = default;
    unit_vec(unit_vec const& other) = default;
    unit_vec& operator=(unit_vec const& other) = default;

    unit_vec(std::array<T, N> const& components)
        : vec<T, N>{components}
    {
        T l = vec<T, N>::length();
        if ((std::abs(l - 1.0f) > std::numeric_limits<T>::epsilon()) && (l > std::numeric_limits<T>::epsilon()))
        {
            T const& one_on_length = (1.0f / l);
            for (auto it = 0; it < N; it++)
                this->m_components[it] *= one_on_length;
        }
    }

    unit_vec(vec<T, N> const& other)
        : unit_vec{other.normalize()}
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
    unit_vec normalize() const override { return (*this); }
};

template <class T, unsigned N> unit_vec<T, N> vec<T, N>::normalize() const { return unit_vec<T, N>{m_components}; }

/**
 * @brief Three-dimensional vector of floats.
 */
using vec3f = vec<float, 3u>;

/**
 * @brief Unit three-dimensional vector of floats.
 */
using unit_vec3f = unit_vec<float, 3u>;
