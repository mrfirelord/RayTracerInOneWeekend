#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

using namespace Walnut;

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
		viewportWidth = ImGui::GetContentRegionAvail().x;
		viewportHeight = ImGui::GetContentRegionAvail().y;

		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)viewportWidth, (float)viewportHeight });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render() {
		if (!image/* || viewportWidth != image->GetWidth() || viewportHeight != image->GetHeight()*/) {
			image = std::make_shared<Image>(viewportWidth, viewportHeight, ImageFormat::RGBA);
			delete[] imageData;
			imageData = new uint32_t[viewportWidth * viewportHeight];
		}

		for (int i = 0; i < viewportWidth * viewportHeight; i++)
			imageData[i] = 0xffff00ff;


		image->SetData(imageData);
	}

private:
	std::shared_ptr<Image> image;
	uint32_t viewportWidth;
	uint32_t viewportHeight;
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