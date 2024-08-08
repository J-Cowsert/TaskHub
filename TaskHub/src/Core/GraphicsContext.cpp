#include "GraphicsContext.h"
#include "Assert.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace taskhub {

	GraphicsContext::GraphicsContext(GLFWwindow* contextWindow) 
		: m_WindowHandle(contextWindow) {

		HUB_CORE_ASSERT(contextWindow, "Context window is null");
	}
	
	void GraphicsContext::Init() {

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HUB_CORE_ASSERT(status, "Failed to initialize Glad!");
		 
		HUB_CORE_INFO("OpenGL Info:");
		HUB_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		HUB_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		HUB_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
	}

	void GraphicsContext::SwapBuffers() {
		
		glfwSwapBuffers(m_WindowHandle);
	}
}