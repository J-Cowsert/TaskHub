#pragma once
#include "Image.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <memory>

namespace taskhub::UI {

	void ShiftCursor(float x, float y);
	void ShiftCursorX(float distance);
	void ShiftCursorY(float distance);
	void AlignCursorForWidth(float width, float alignment);
	void AlignCursorForHeight(float height, float alignment);
}