#pragma once
#include "Core/Application.h"

int main()
{
	taskhub::Application* App = taskhub::CreateApplication();
	App->Run();
	delete App;
}