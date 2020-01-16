#include "stdafx.h"

void InspectorDrawer::Text(String_t name, String_t text) {
	float width = PrepareLine(name);
	ImGui::SameLine(width - ImGui::CalcTextSize(text, NULL, true).x);
	ImGui::LabelText(Format_t("##%s", name), text);
}

bool InspectorDrawer::EditText(GameObject* gameObject, String_t name, String& text) {
	static char buffer[256];
	strcpy(buffer, text.c_str());
	float width = PrepareLine(name);
	ImGui::SameLine(width - ImGui::CalcTextSize(buffer, NULL, true).x);
	//Undo::Record(gameObject);
	bool toRet = ImGui::InputText(Format_t("##%s", name), buffer, 256);
	if (toRet) {
		text = buffer;
		//Undo::FinishRecording(false);
	}// else Undo::CancelRecording();
	return toRet;
}

bool InspectorDrawer::Vec2(GameObject* gameObject, String_t name, Vector2& vec) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	//Undo::Record(gameObject);
	bool toRet = ImGui::InputFloat2(Format_t("##%s", name), (float*)&vec);
	//if (toRet) Undo::FinishRecording(false);
	//else Undo::CancelRecording();
	return toRet;
}

bool InspectorDrawer::Float(GameObject* gameObject, String_t name, float& flt) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	//Undo::Record(gameObject);
	bool toRet = ImGui::InputFloat(Format_t("##%s", name), (float*)&flt);
	//if (toRet) Undo::FinishRecording(false);
	//else Undo::CancelRecording();
	return toRet;
}

bool InspectorDrawer::Vec3(GameObject* gameObject, String_t name, Vector3& vec) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	//Undo::Record(gameObject);
	bool toRet = ImGui::InputFloat3(Format_t("##%s", name), (float*)&vec);
	//if (toRet) Undo::FinishRecording(false);
	//else Undo::CancelRecording();
	return toRet;
}

bool InspectorDrawer::Color(GameObject* gameObject, String_t name, ::Color& color) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	//Undo::Record(gameObject);
	bool toRet = ImGui::ColorEdit4(Format_t("##%s", name), (float*)&color);
	//if (toRet) Undo::FinishRecording(false);
	//else Undo::CancelRecording();
	return toRet;
}

bool InspectorDrawer::Combo(GameObject* gameObject, String_t name, int* item, String_t const items[], int itemCount) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	//Undo::Record(gameObject);
	bool toRet = ImGui::Combo(Format_t("##%s", name), item, items, itemCount);
	//if (toRet) Undo::FinishRecording(false);
	//else Undo::CancelRecording();
	return toRet;
}

void InspectorDrawer::Texture(GameObject* gameObject, String_t name, StreamedTexture* texture) {
	float width = PrepareLine(name);
	ImGui::SameLine(width / 2);
	ImGui::PushItemWidth(width / 2);
	ImGui::Image(texture->GetTexture()->GetImGuiHandle(), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
}