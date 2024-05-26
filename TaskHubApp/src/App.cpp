#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/EntryPoint.h"



int main()
{
	taskhub::ApplicationProvision prov;
	taskhub::Application* App = new taskhub::Application(prov);
	App->Run();

	delete App;
}