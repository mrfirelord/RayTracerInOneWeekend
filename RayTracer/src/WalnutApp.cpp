#include <glm/glm.hpp>
#include <iostream>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Ray.h"
#include "rtweekend.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

using namespace Walnut;

glm::vec3 getNormalizedRayColor(const Ray& r, const Hittable& world) {
	HitRecord rec;
	if (world.hit(r, 0, INF, rec)) {
		return 0.5f * (rec.normal + glm::vec3(1.0f, 1.0f, 1.0f));
	}

	glm::vec3 unitDirection = glm::normalize(r.direction());
	// y: [-1,1] -> [0,1]
	auto t = 0.5f * (unitDirection.y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

// transform [0-1, 0-1, 0-1] to 0xff[00-ff][00-ff][00-ff]
uint32_t toColor(glm::vec3 normalizedColor, int numberOfSamples) {
	glm::vec3 color = normalizedColor / float(numberOfSamples) * 255.999f;
	return (0xff << 24) | (((uint8_t)color.b) << 16) | (((uint8_t)color.g) << 8) | ((uint8_t)color.r);
}

class ExampleLayer : public Walnut::Layer {
public:
	virtual void OnUIRender() override {
		ImGui::Begin("Config");
		if (ImGui::Button("Render"))
			Render();
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		imageWidth = ImGui::GetContentRegionAvail().x;
		imageHeight = ImGui::GetContentRegionAvail().y;
		aspectRatio = float(imageWidth) / imageHeight;

		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)imageWidth, (float)imageHeight });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render() {
		if (!image/* || viewportWidth != image->GetWidth() || viewportHeight != image->GetHeight()*/) {
			image = std::make_shared<Image>(imageWidth, imageHeight, ImageFormat::RGBA);
			delete[] imageData;
			imageData = new uint32_t[imageWidth * imageHeight];
		}

		const int samplesPerPixel = 10;

		// Camera

		Camera camera(aspectRatio * 2.0, 2.0);

		// World

		HittableList world;
		world.add(make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
		world.add(make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));

		for (int verticalPixel = 0; verticalPixel < imageHeight; verticalPixel++) {
			for (int horizontalPixel = 0; horizontalPixel < imageWidth; horizontalPixel++) {
				glm::vec3 pixelColor(0.0f, 0.0f, 0.0f);
				for (int s = 0; s < samplesPerPixel; s++) {
					auto u = (horizontalPixel + randomFloat()) / (imageWidth - 1);
					auto v = (verticalPixel + randomFloat()) / (imageHeight - 1);
					
					Ray r = camera.getRay(u, v);
					pixelColor += getNormalizedRayColor(r, world);
				}

				imageData[imageWidth * imageHeight - (verticalPixel * imageWidth + horizontalPixel)] = toColor(pixelColor, samplesPerPixel);
			}
		}

		image->SetData(imageData);
	}

private:
	std::shared_ptr<Image> image;
	uint32_t imageWidth;
	uint32_t imageHeight;
	float aspectRatio;
	uint32_t* imageData = nullptr;
};

Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Fire RayTracer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]() {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				app->Close();
			}
			ImGui::EndMenu();
		}
		});
	return app;
}