#include "CustomWidgets.h"
#include "Core/Assert.h"

namespace taskhub::UI {

	// TODO: Find out why tint color doesnt work on certain images
	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   const std::shared_ptr<taskhub::Image>& imageHovered,
		                   const std::shared_ptr<taskhub::Image>& imagePressed,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax) {

		auto* drawList = ImGui::GetWindowDrawList();

		if (ImGui::IsItemActive())
			drawList->AddImage((void*)(intptr_t)imagePressed->GetTextureID(), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintPressed);
		else if (ImGui::IsItemHovered())
			drawList->AddImage((void*)(intptr_t)imageHovered->GetTextureID(), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
		else
			drawList->AddImage((void*)(intptr_t)image->GetTextureID(), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tint);
	}

	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax) {
		RenderImageButton(image, image, image, tint, tintHovered, tintPressed, rectMin, rectMax);
	}

	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed) {
		RenderImageButton(image, image, image, tint, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	void CircularProgressBar(float radius, float thickness, float progress, const std::string& text, ImU32 col) {

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 center = ImVec2(ImGui::GetCursorScreenPos().x + radius, ImGui::GetCursorScreenPos().y + radius);

		const float startAngle = -IM_PI / 2;
		const float endAngle = startAngle - progress * 2.0f * IM_PI;

		// Draw background circle outline
		draw_list->AddCircle(center, radius, IM_COL32(100, 100, 100, 255), 100, thickness);

		HUB_CORE_ASSERT(progress >= 0.0f && progress <= 1.0f, "Progress should always be in between 0 and 1.")
		if (progress > 0.0f) {
			draw_list->PathArcTo(center, radius, startAngle, endAngle, 100);
			draw_list->PathStroke(col, ImDrawFlags_RoundCornersAll, thickness + 0.5f);
		}

		ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
		ImVec2 text_pos(center.x - text_size.x * 0.5f, center.y - text_size.y * 0.5f);
		draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), text.c_str());

		// Advance the cursor position
		ImGui::Dummy(ImVec2(radius * 2, radius * 2));
	}
}