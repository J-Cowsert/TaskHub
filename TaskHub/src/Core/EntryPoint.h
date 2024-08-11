#pragma once
#include "Core/Application.h"

int main(int argc, char** argv) {

	taskhub::Log::Init();
	taskhub::Application* App = taskhub::CreateApplication();
	App->Run();
	delete App;
}