#pragma once
//#include "imgui.h"
#include <string>
#include <vector>
#include <memory>

struct GLFWwindow;

namespace taskhub {

	struct ApplicationProvision {
		std::string Name = "TaskHub";
		uint32_t Width = 1600;
		uint32_t Height = 900;
	};

	class Application {
	public:
		Application(const ApplicationProvision& provision);
		~Application();

		void Run();

		GLFWwindow* GetWindow() const { return m_Window; }

	private:
		void Init();
		void Shutdown();

	private:

		GLFWwindow* m_Window = nullptr;
		ApplicationProvision m_AppProvision;
		bool m_Running = true;
	};

	Application* CreateApplication();
}