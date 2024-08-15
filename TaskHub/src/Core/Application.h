#pragma once
#include "Base.h"
#include "Layer.h"
#include "GUI/Image.h"
#include "GraphicsContext.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <functional>

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

		void SetMenubarCallback(const std::function<void()>& callback) { m_MenuBar = callback; }

		static Application& Get();
		GLFWwindow* GetWindow() const { return m_Window; }

		bool IsMinimized() const;

	private:
		void Init();
		void Shutdown();

	private:
		GLFWwindow* m_Window = nullptr;
		std::unique_ptr<GraphicsContext> m_Context;

	private:
		ApplicationProvision m_AppProvision;
		bool m_Running = true;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
		std::function<void()> m_MenuBar;
	};

	// To be defined in client app
	Application* CreateApplication();
}