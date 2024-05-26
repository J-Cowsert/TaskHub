#pragma once

#include "Application.h"

extern  taskhub::Application* taskhub::CreateApplication();

taskhub::Application* taskhub::CreateApplication() {

	auto app = taskhub::CreateApplication();
	app->Run();
	delete app;

	return app;
}