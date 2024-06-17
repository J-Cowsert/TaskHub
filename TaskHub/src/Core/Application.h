#pragma once
#include "Log.h"
#include "Layer.h"
#include "imgui.h"
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

struct GLFWwindow;

namespace taskhub {

	struct ApplicationProvision {

		std::string Name = "TaskHub";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		std::filesystem::path AppIconPath;
	};

	class Application {
	public:
		Application(const ApplicationProvision& provision);
		~Application();

		void Run();
		void Close();

		template<typename T> 
		void PushLayer() {
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer");
			m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
		}

		void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }

		static Application& Get();
		GLFWwindow* GetWindow() const { return m_Window; }

	private:
		void Init();
		void Shutdown();

	private:
		GLFWwindow* m_Window = nullptr;
		ApplicationProvision m_AppProvision;
		bool m_Running = true;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	};

	// To be defined in client app
	Application* CreateApplication();
}