#include "UIUtils.h"
#include "Core/Assert.h"

namespace taskhub::UI {

	void ShiftCursor(float x, float y) {

		const ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
	}

	void ShiftCursorX(float distance) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
	}

	void ShiftCursorY(float distance) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
	}

	void AlignCursorForWidth(float width, float alignment = 0.5f) {

		float availWidth = ImGui::GetContentRegionAvail().x;
		float offsetX = (availWidth - width) * alignment;
		if (offsetX > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
	}

	void AlignCursorForHeight(float height, float alignment = 0.5f) {

		float availHeight = ImGui::GetContentRegionAvail().y;
		float offsetY = (availHeight - height) * alignment;
		if (offsetY > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosY() + offsetY);
	}

	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image, 
		                   const std::shared_ptr<taskhub::Image>& imageHovered,
		                   const std::shared_ptr<taskhub::Image>& imagePressed,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax) 
	{
		auto* drawList = ImGui::GetForegroundDrawList();

		if (ImGui::IsItemActive())
			drawList->AddImage((void*)(intptr_t)imagePressed->GetTextureID(), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintPressed);
		else if (ImGui::IsItemHovered())	 
			drawList->AddImage((void*)(intptr_t)imageHovered->GetTextureID(), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
		else								 
			drawList->AddImage((void*)(intptr_t)image->GetTextureID(), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tint);
	}

	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image, 
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax) 
	{
		RenderImageButton(image, image, image, tint, tintHovered, tintPressed, rectMin, rectMax);
	}
}