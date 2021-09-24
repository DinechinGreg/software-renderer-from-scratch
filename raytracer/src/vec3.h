#pragma once

template <class T> class unit_vec3;

template <class T> class vec3
{
  public:
    vec3(T x_in, T y_in, T z_in);
    vec3(T xyz);
    virtual ~vec3() = default;
    vec3(vec3 const& other) = default;
    vec3& operator=(vec3 const& other) = default;

    vec3 operator+(vec3 const& other) const;
    vec3 operator-(vec3 const& other) const;
    vec3 operator-() const;

    T dot(vec3 const& other) const;
    virtual T length() const;
    virtual unit_vec3<T> normalize() const;

    static vec3<T> const& zero()
    {
        static vec3<T> o{0.0};
        return o;
    }

    static vec3<T> const& one()
    {
        static vec3<T> i{1.0};
        return i;
    }

    T x;
    T y;
    T z;
};

template <typename T> vec3<T> operator*(T const& scalar, vec3<T> const& vec);
