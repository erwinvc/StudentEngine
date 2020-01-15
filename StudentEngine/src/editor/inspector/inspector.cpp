#include "stdafx.h"

void InspectorDrawer::Text(String_t name, String_t text) {
	float width = PrepareLine(name);
	ImGui::SameLine(width - ImGui::CalcTextSize(text, NULL, true).x);
	ImGui::LabelText(Format_t("##%s", name), text);
}

bool InspectorDrawer::EditText(String_t name, String& text) {
	static char buffer[256];
	strcpy(buffer, text.c_str());
	float width = PrepareLine(name);
	ImGui::SameLine(width - ImGui::CalcTextSize(buffer, NULL, true).x);
	if (ImGui::InputText(Format_t("##%s", name), buffer, 256)) {
		text = buffer;
		return true;
	}
	return false;
}

bool InspectorDrawer::Vec2(String_t name, Vector2& vec) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	return ImGui::InputFloat2(Format_t("##%s", name), (float*)&vec);
}

bool InspectorDrawer::Float(String_t name, float& flt) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	return ImGui::InputFloat(Format_t("##%s", name), (float*)&flt);
}

bool InspectorDrawer::Vec3(String_t name, Vector3& vec) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	return ImGui::InputFloat3(Format_t("##%s", name), (float*)&vec);
}

void InspectorDrawer::Color(String_t name, ::Color& color) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	ImGui::ColorEdit4(Format_t("##%s", name), (float*)&color);
}

bool InspectorDrawer::Combo(String_t name, int* item, String_t const items[], int itemCount) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	return ImGui::Combo(Format_t("##%s", name), item, items, itemCount);
}

void InspectorDrawer::Texture(String_t name, StreamedTexture* texture) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	ImGui::Image(texture->GetTexture()->GetImGuiHandle(), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
}