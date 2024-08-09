#pragma once
#include "Image.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <memory>

namespace taskhub::UI {


	/// Renders an image button with different images for different states (normal, hovered, and pressed).
	/// 
	/// This function draws an image button with the provided images and tint colors.
	/// It will use the `imagePressed` when the button is active, `imageHovered` when hovered, and `image` for the default state.
	/// Best used with ImGui::InvisableButton() as such:
	/// 
	/// if (ImGui::InvisibleButton("str_id", ImVec2(Image->GetWidth(), Image->GetHeight()))) {
	///      
	///	    //Code to run when ImageButton is pressed
    /// }
    /// taskhub::UI::RenderImageButton(Image, ImageHovered, ImagePressed, Tint, TintHovered, TintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	/// 
	/// Parameters:
	///		image - The image to display when the button is in its default state.
	///		imageHovered - The image to display when the button is hovered over.
	///		imagePressed - The image to display when the button is pressed.
	///		tint - The tint color applied to the default state image.
	///		tintHovered - The tint color applied to the hovered state image.
	///		tintPressed - The tint color applied to the pressed state image.
	///		rectMin - The minimum coordinate (bottom-left) of the button's rectangle.
	///		rectMax - The maximum coordinate (top-right) of the button's rectangle.
	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   const std::shared_ptr<taskhub::Image>& imageHovered,
		                   const std::shared_ptr<taskhub::Image>& imagePressed,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax);

	/// Renders an image button with the same image for all states.

	/// Parameters:
	///		image - The image displayed
	///		tint - The tint color applied to the default state image.
	///		tintHovered - The tint color applied to the hovered state image.
	///		tintPressed - The tint color applied to the pressed state image.
	///		rectMin - The minimum coordinate (bottom-left) of the button's rectangle.
	///		rectMax - The maximum coordinate (top-right) of the button's rectangle.
	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed,
		                   ImVec2 rectMin, ImVec2 rectMax);

	/// Renders an image button with the same image for all states and uses the current ImGui item rectangle for positioning.
	/// 
	/// Parameters:
	///		image - The image to display in all states (default, hovered, pressed).
	///		tint - The tint color applied to the image in the default state.
	///		tintHovered - The tint color applied to the image in the hovered state.
	///		tintPressed - The tint color applied to the image in the pressed state.
	void RenderImageButton(const std::shared_ptr<taskhub::Image>& image,
		                   ImU32 tint, ImU32 tintHovered, ImU32 tintPressed);

	/// This function draws a circular progress bar with the specified radius, thickness, and progress value.
	/// It also renders optional text in the center of the progress bar.
	/// 
	/// Parameters:
	///		radius - The radius of the progress bar.
	///		thickness - The thickness of the progress bar stroke.
	///		progress - A value between 0.0f and 1.0f representing the progress (completion level).
	///		text - The text to display in the center of the progress bar.
	///		col - The color of the progress bar stroke.
	void CircularProgressBar(float radius, float thickness, float progress, const std::string& text, ImU32 col);
}