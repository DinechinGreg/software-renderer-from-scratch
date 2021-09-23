#include "vec3.h"

#include <cmath>

template <typename T> vec3<T>::vec3(T x_in, T y_in, T z_in) : x{x_in}, y{y_in}, z{z_in}
{
}

template <typename T> vec3<T>::vec3(T xyz) : vec3{xyz, xyz, xyz}
{
}

template <typename T> vec3<T> vec3<T>::operator+(vec3<T> const &other) const
{
    return vec3(x + other.x, y + other.y, z + other.z);
}

template <typename T> vec3<T> vec3<T>::operator-(vec3<T> const &other) const
{
    return operator+(-other);
}

template <typename T> vec3<T> vec3<T>::operator-() const
{
    return -1.0f * (*this);
}

template <typename T> T vec3<T>::dot(vec3<T> const &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

template <typename T> T vec3<T>::length() const
{
    return std::sqrt(this->dot(*this));
}

template <typename T> vec3<T> vec3<T>::normalize() const
{
    return (1.0f / length()) * (*this);
}

template <typename T> vec3<T> operator*(T const &scalar, vec3<T> const &vec)
{
    return vec3(scalar * vec.x, scalar * vec.y, scalar * vec.z);
}

template vec3<float>::vec3(float x_in, float y_in, float z_in);
template vec3<float>::vec3(float xyz);
template vec3<float> vec3<float>::operator+(vec3<float> const &other) const;
template vec3<float> vec3<float>::operator-(vec3<float> const &other) const;
template vec3<float> vec3<float>::operator-() const;
template float vec3<float>::dot(vec3<float> const &other) const;
template float vec3<float>::length() const;
template vec3<float> vec3<float>::normalize() const;
template vec3<float> operator*(float const &scalar, vec3<float> const &vec);
