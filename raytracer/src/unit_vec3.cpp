#include "unit_vec3.h"

#include <cmath>
#include <limits>

template <typename T>
unit_vec3<T>::unit_vec3(T x_in, T y_in, T z_in)
    : vec3<T>{x_in, y_in, z_in}
{
    T length = vec3<T>::length();
    if (std::abs(length - 1.0f) > std::numeric_limits<T>::epsilon() && length != 0.0f)
        *this = (1.0f / length) * (*this);
}

template <typename T>
unit_vec3<T>::unit_vec3(T xyz)
    : unit_vec3{xyz, xyz, xyz}
{
}

template <typename T>
unit_vec3<T>::unit_vec3(vec3<T> const& other)
    : unit_vec3{other.x, other.y, other.z}
{
}

template <typename T> T unit_vec3<T>::length() const { return 1.0f; }

template <typename T> unit_vec3<T> unit_vec3<T>::normalize() const { return (*this); }

template unit_vec3<float>::unit_vec3(float x_in, float y_in, float z_in);
template unit_vec3<float>::unit_vec3(float xyz);
template unit_vec3<float>::unit_vec3(vec3<float> const& other);
template float unit_vec3<float>::length() const;
template unit_vec3<float> unit_vec3<float>::normalize() const;
