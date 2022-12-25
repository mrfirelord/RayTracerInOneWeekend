#pragma once

#include <glm/glm.hpp>
#include "hittable.h"
#include "Ray.h"

class Sphere : public Hittable {
public:
	Sphere() {}
	Sphere(glm::vec3 cen, float r) : center(cen), radius(r) {};

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

public:
	glm::vec3 center;
	float radius;
};

bool Sphere::hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const {
	glm::vec3 origin = ray.origin() - center;

	float a = glm::dot(ray.direction(), ray.direction());
	float halfB = glm::dot(origin, ray.direction());
	float c = glm::dot(origin, origin) - radius * radius;
	auto discriminant = halfB * halfB - a * c;

	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-halfB - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-halfB + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = ray.at(rec.t);
	rec.setFrontFace(ray, (rec.p - center) / radius);

	return true;
}
