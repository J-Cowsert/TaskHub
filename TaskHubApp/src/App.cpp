
#include "Core/Application.h"
#include "Core/EntryPoint.h"


taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;
	provision.Name = "Test";
	taskhub::Application* app = new taskhub::Application(provision);

	return app;
}