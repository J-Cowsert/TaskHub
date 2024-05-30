#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

struct ImVec2;

namespace taskhub {

	class Input {
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode mousecode);
		static ImVec2 GetMousePos();
	};
}