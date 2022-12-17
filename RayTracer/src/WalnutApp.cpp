#include <glm/glm.hpp>
#include <iostream>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Color.h"
#include "Ray.h"

using namespace Walnut;

glm::vec3 getNormalizedRayColor(const Ray& r) {
	glm::vec3 unitDirection = glm::normalize(r.direction());
	// y: [-1,1] -> [0,1]
	float t = 0.5 * (unitDirection.y + 1.0);
	return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

// transform [0-1, 0-1, 0-1] to 0xff[00-ff][00-ff][00-ff]
uint32_t toColor(glm::vec3 normalizedColor) {
	glm::vec3 color = normalizedColor * 255.999f;
	return (0xff << 24) | (((uint8_t)color.b) << 16) | (((uint8_t)color.g) << 8) | ((uint8_t)color.r);
}

class ExampleLayer : public Walnut::Layer {
public:
	virtual void OnUIRender() override {
		ImGui::Begin("Config");
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		imageWidth = ImGui::GetContentRegionAvail().x;
		imageHeight = ImGui::GetContentRegionAvail().y;
		aspectRatio = imageWidth / imageHeight;

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

		// Camera

		float viewportHeight = 2.0;
		float viewportWidth = aspectRatio * viewportHeight;
		float focalLength = 1.0;

		auto origin = glm::vec3(0, 0, 0);
		glm::vec3 horizontal = glm::vec3(viewportWidth, 0.0f, 0.0f);
		glm::vec3 vertical = glm::vec3(0.0f, viewportHeight, 0.0f);
		glm::vec3 lowerLeftCorner = origin - horizontal / 2.0f - vertical / 2.0f - glm::vec3(0.0f, 0.0f, focalLength);

		for (int verticalPixel = 0; verticalPixel < imageHeight; verticalPixel++) {
			for (int horizontalPixel = 0; horizontalPixel < imageWidth; horizontalPixel++) {
				float u = float(horizontalPixel) / (imageWidth - 1);
				float v = float(verticalPixel) / (imageHeight - 1);

				Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
				auto normalizedColor = getNormalizedRayColor(ray);

				imageData[imageWidth * imageHeight - (verticalPixel * imageWidth + horizontalPixel)] = toColor(normalizedColor);
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