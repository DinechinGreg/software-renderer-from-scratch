#pragma once

#include <cmath>
#include <limits>
#include <random>

namespace math
{

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

} // namespace math
