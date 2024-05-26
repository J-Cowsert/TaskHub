
#include "Core/Application.h"
#include "Core/EntryPoint.h"

class ToolDemo : public taskhub::Layer {
public:

	void OnUIRender() override {
		ImGui::ShowDemoWindow();
		ImGui::ShowStyleEditor();
	}
};

taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;
	provision.Name = "Test";

	taskhub::Application* app = new taskhub::Application(provision);
	app->PushLayer<ToolDemo>();

	return app;
}