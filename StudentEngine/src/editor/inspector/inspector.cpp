#include "stdafx.h"

void InspectorDrawer::Text(String_t name, String_t text) {
	float width = PrepareLine(name);
	ImGui::SameLine(width - ImGui::CalcTextSize(text, NULL, true).x);
	ImGui::LabelText(Format_t("##%s", name), text);
}
//static void Text(const String& name, const String& text) { Text(name.c_str(), text.c_str()); }

void InspectorDrawer::Color(String_t name, ::Color& color) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	ImGui::ColorEdit4(Format_t("##%s", name), (float*)&color);
}
//static void Color(const String& name, ::Color& color) { Color(name.c_str(), color); }

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
	ImGui::Image((void*)texture->GetTexture()->GetHandle(), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
}