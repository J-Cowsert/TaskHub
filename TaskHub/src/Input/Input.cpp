#include "Input.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"

namespace taskhub {

    bool Input::IsKeyPressed(KeyCode key) {

        GLFWwindow* window = Application::Get().GetWindow();
        int state = glfwGetKey(window, key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode mousecode) {

        GLFWwindow* window = Application::Get().GetWindow();
        int state = glfwGetMouseButton(window, mousecode);
        return state == GLFW_PRESS;
    }

    ImVec2 Input::GetMousePos() {

        GLFWwindow* window = Application::Get().GetWindow();
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { (float)x, (float)y };
    }
}