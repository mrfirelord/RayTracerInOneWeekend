#pragma once

#include "rtweekend.h"

class Camera {
public:
    Camera(float viewportWidth, float viewportHeight) : viewportWidth(viewportWidth), viewportHeight(viewportHeight) {
        float aspectRatio = 16.0f / 9.0f;
        float focalLength = 1.0;

        origin = glm::vec3(0.0f, 0.0f, 0.0f);
        horizontal = glm::vec3(viewportWidth, 0.0f, 0.0f);
        vertical = glm::vec3(0.0, viewportHeight, 0.0);
        lowerLeftCorner = origin - horizontal / 2.0f - vertical / 2.0f - glm::vec3(0, 0, focalLength);
    }

    Ray getRay(float u, float v) const {
        return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

private:
    glm::vec3 origin;
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    float viewportWidth;
    float viewportHeight;
};