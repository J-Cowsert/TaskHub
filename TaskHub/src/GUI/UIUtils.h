#pragma once
#include "imgui.h"
#include "imgui_internal.h"

namespace taskhub::UI {

	void ShiftCursor(float x, float y);
	void ShiftCursorX(float distance);
	void ShiftCursorY(float distance);

	/// This function positions the cursor horizontally within the available width of the
	/// content region, according to the specified alignment. The alignment parameter
	/// determines where the cursor will be placed relative to the available space.
	/// 
	/// Parameters:
	///		width - The width of the element that needs to be aligned.
	///		alignment - A value between 0.0f (left alignment) and 1.0f (right alignment), with 0.5f representing center alignment. Default is 0.5f.
	void AlignCursorForWidth(float width, float alignment = 0.5f);

	/// This function positions the cursor vertically within the available height of the
	/// content region, according to the specified alignment. The alignment parameter
	/// determines where the cursor will be placed relative to the available space.
	/// 
	/// Parameters:
	///		width - The width of the element that needs to be aligned.
	///		alignment - A value between 0.0f (top alignment) and 1.0f (bottom alignment), with 0.5f representing center alignment. Default is 0.5f.
	void AlignCursorForHeight(float height, float alignment = 0.5f);
}