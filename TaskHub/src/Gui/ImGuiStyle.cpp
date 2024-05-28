#include "ImGuiStyle.h"

namespace taskhub::UI {

	void SetTaskHubTheme() {

		auto& style = ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;


		// Buttons
		colors[ImGuiCol_Button] = ImColor(56, 56, 56, 200);
		colors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255);
		colors[ImGuiCol_ButtonActive] = ImColor(56, 56, 56, 150);

		colors[ImGuiCol_TabHovered] = ImColor(255, 225, 135, 30);
		colors[ImGuiCol_TabActive] = ImColor(255, 225, 135, 60);
	}
}