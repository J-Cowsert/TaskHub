#pragma once

struct GLFWwindow;

namespace taskhub {

	class GraphicsContext {
	public:
		GraphicsContext(GLFWwindow* contextWindow);

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_WindowHandle;
	};
}