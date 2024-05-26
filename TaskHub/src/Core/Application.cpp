#include "Application.h"

#include <GLFW/glfw3.h>
//#include <stdio.h>    // printf, fprintf
#include <iostream>
#pragma once

static void glfw_error_callback(int error, const char* description) {

	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace taskhub {

	Application::Application(const ApplicationProvision& provision) {
		Init();
	}

	Application::~Application() {
		Shutdown();
	}

	void Application::Run() {

		while (!glfwWindowShouldClose(m_Window) && m_Running)
		{
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();
		}
	}

	void Application::Init() {

		glfwSetErrorCallback(glfw_error_callback);

		if (!glfwInit()) {
			std::cout << "glfw failed to init";
			return;
		}
		m_Window = glfwCreateWindow(m_AppProvision.Width, m_AppProvision.Height, m_AppProvision.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
	
	}

	void Application::Shutdown() {

	}
}