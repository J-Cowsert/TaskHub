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

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 center = ImVec2(ImGui::GetCursorScreenPos().x + radius, ImGui::GetCursorScreenPos().y + radius);

		const float startAngle = -IM_PI / 2;

		progress = std::clamp(progress, 0.0f, 1.0f);
		HUB_CORE_ASSERT(progress >= 0.0f && progress <= 1.0f, "Progress should always be in between 0 and 1.");

		const float endAngle = startAngle - progress * 2.0f * IM_PI;

		// Draw background circle outline
		drawList->AddCircle(center, radius, IM_COL32(100, 100, 100, 255), 100, thickness);

		if (progress > 0.0f) {
			drawList->PathArcTo(center, radius, startAngle, endAngle, 100);
			drawList->PathStroke(col, ImDrawFlags_RoundCornersAll, thickness + 0.5f);
		}

		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		ImVec2 textPos(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f);
		drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), text.c_str());

		// Advance the cursor position
		ImGui::Dummy(ImVec2(radius * 2, radius * 2));
	}

	void ToggleButton(const char* str_id, bool* v) {

			
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked())
			*v = !*v;

		float t = *v ? 1.0f : 0.0f;

		ImGuiContext& g = *GImGui;
		float ANIM_SPEED = 0.08f;
		if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
		{
			float tAnim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = *v ? (tAnim) : (1.0f - tAnim);
		}

		ImU32 colBg;
		if (ImGui::IsItemHovered())
			colBg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
		else
			colBg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

		drawList->AddRectFilled(p, ImVec2(p.x + width, p.y + height), colBg, height * 0.5f);
		drawList->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}
}