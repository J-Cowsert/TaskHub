#pragma once
#include "Image.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <memory>

namespace taskhub::UI {

	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   const std::shared_ptr<taskhub::Image>& imageHovered,
		                   const std::shared_ptr<taskhub::Image>& imagePressed,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax);
	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax);
	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed);

	void CircularProgressBar(float radius, float thickness, float progress, const std::string& text, ImU32 col);
}