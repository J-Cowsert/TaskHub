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
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
	}
}