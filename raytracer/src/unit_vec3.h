#pragma once

#include <vec3.h>

template <class T> class unit_vec3 : public vec3<T>
{
  public:
    unit_vec3(T x_in, T y_in, T z_in);
    unit_vec3(T xyz);
    unit_vec3(vec3<T> const& other);
    ~unit_vec3() = default;
    unit_vec3(unit_vec3 const& other) = default;
    unit_vec3& operator=(unit_vec3 const& other) = default;

    T length() const override;
    unit_vec3<T> normalize() const override;
};
