#pragma once


#include "Core/Core.h"
#include "Core/Application.h"


int main()
{
	taskhub::Application* App = taskhub::CreateApplication();
	App->Run();
	delete App;
}
