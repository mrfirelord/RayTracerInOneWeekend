#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

#include <random>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

inline double random_float() {
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

// Common Headers

#include "ray.h"