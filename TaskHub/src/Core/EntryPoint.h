#pragma once
#include "Core/Application.h"

int main(int argc, char** argv) {

	taskhub::Log::Init();
	HUB_CORE_INFO("Log::Init in entrypoint\n");

	taskhub::Application* App = taskhub::CreateApplication();
	App->Run();
	delete App;
}