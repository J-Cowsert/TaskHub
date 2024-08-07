#pragma once
#include "imgui.h"
#include "imgui_internal.h"

namespace taskhub::UI {

	void ShiftCursor(float x, float y);
	void ShiftCursorX(float distance);
	void ShiftCursorY(float distance);
	void AlignCursorForWidth(float width, float alignment = 0.5f);
	void AlignCursorForHeight(float height, float alignment = 0.5f);
}