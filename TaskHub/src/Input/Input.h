#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace taskhub {

	class Input {

		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode mousecode);
		static ImVec2 GetMousePos();
	};
}