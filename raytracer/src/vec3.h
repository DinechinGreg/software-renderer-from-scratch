#pragma once

template <class T> class vec3
{
  public:
    vec3(T x_in, T y_in, T z_in);
    vec3(T xyz);
    ~vec3() = default;
    vec3(vec3 const &other) = default;
    vec3 &operator=(vec3 const &other) = default;

    vec3 operator+(vec3 const &other) const;
    vec3 operator-(vec3 const &other) const;
    vec3 operator-() const;

    T dot(vec3 const &other) const;
    T length() const;
    vec3 normalize() const;

    T x;
    T y;
    T z;
};

template <typename T> vec3<T> operator*(T const &scalar, vec3<T> const &vec);
