#pragma once

#include <glm/glm.hpp>
#include "ray.h"

struct HitRecord {
	glm::vec3 p;
	glm::vec3 normal;
	float t;
	bool frontFace;

	inline void setFrontFace(const Ray& ray, const glm::vec3& outwardNormal) {
		frontFace = glm::dot(ray.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const = 0;
};

