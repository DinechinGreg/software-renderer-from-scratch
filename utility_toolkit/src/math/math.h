#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

namespace math
{

constexpr float pi = static_cast<float>(M_PI);

/**
 * @brief Returns a default epsilon float value.
 * @return The numeric limits epsilon.
 */
static float numeric_epsilon() { return std::numeric_limits<float>::epsilon(); }

/**
 * @brief Returns a default infinity float value.
 * @return The numeric limits infinity.
 */
static float numeric_infinity() { return std::numeric_limits<float>::infinity(); }

/**
 * @brief Returns a custom epsilon, as pow(base, exponent) * mult_factor, to be used e.g. when the epsilon should vary based on a distance value.
 * @param[in] base. Base for the power computation.
 * @param[in] exponent. Exponent for the power computation.
 * @param[in] mult_factor. Multiplication factor.
 * @return The custom epsilon, as a float.
 */
static float distance_epsilon(float base, float exponent = 1.0f, float mult_factor = 1e-4f) { return std::pow(base, exponent) * mult_factor; }

/**
 * @brief Generates a random floating point number in range [0,1[.
 * @return The generated number, as a float.
 */
static float generate_random_01()
{
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    return std::generate_canonical<float, 10>(random_engine);
}

/**
 * @brief Performs linear interpolation between the given values using the given weight.
 * @tparam T. Type of the range values.
 * @tparam U. Type of the weight value.
 * @param[in] start. The start of the range in which to interpolate.
 * @param[in] end. The end of the range in which to interpolate.
 * @param[in] weight. The value to use to interpolate between start and end.
 * @return The interpolated value.
 */
template <typename T, typename U> T linear_interpolation(T start, T end, U weight) { return start * (1.0f - weight) + end * weight; }

/**
 * @brief Clamps the given value in the given range.
 * @tparam T. Type of the values.
 * @param[in] value. The value to clamp.
 * @param[in] start. The start of the range in which to clamp.
 * @param[in] end. The end of the range in which to clamp.
 * @return The clamped value.
 */
template <typename T> T clamp(T value, T start, T end) { return (std::max)(start, (std::min)(end, value)); }

} // namespace math
