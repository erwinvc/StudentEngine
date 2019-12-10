#pragma once

class InspectorDrawer {
private:
	static float PrepareLine(String_t& name) {
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name);
		return ImGui::GetContentRegionAvail().x;
	}
public:
	//static bool Header(const String& name) { return ImGui::CollapsingHeader(name.c_str()); }
	static bool Header(String_t name) { return ImGui::CollapsingHeader(name); }
	static void Text(String_t name, String_t text) {
		float width = PrepareLine(name);
		ImGui::SameLine(width - ImGui::CalcTextSize(text, NULL, true).x);
		ImGui::LabelText(Format_t("##%s", name), text);
	}
	//static void Text(const String& name, const String& text) { Text(name.c_str(), text.c_str()); }

	static void Color(String_t name, ::Color& color) {
		float width = PrepareLine(name);
		ImGui::SameLine(width / 2);
		ImGui::PushItemWidth(width / 2);
		ImGui::ColorEdit4(Format_t("##%s", name), (float*)&color);
	}
	//static void Color(const String& name, ::Color& color) { Color(name.c_str(), color); }

	static bool Combo(String_t name, int* item, String_t const items[], int itemCount) {
		float width = PrepareLine(name);
		ImGui::SameLine(width / 2);
		ImGui::PushItemWidth(width / 2);
		return ImGui::Combo(Format_t("##%s", name), item, items, itemCount);
	}

};

class Inspector : public Singleton<Inspector> {
protected:
	Inspector() {}
	~Inspector() {}
	friend Singleton;

private:
	InspectorDrawable* m_selected = nullptr;

public:
	void OnImGui() {
		if (ImGui::Begin("Inspector", nullptr)) {
			if (m_selected)
				m_selected->InspectorDraw();
		}
		ImGui::End();
	}

	void SetSelected(InspectorDrawable* drawable) {
		m_selected = drawable;
	}
	InspectorDrawable* GetSelected() {
		return m_selected;
	}
	bool IsSelected(InspectorDrawable* drawable) {
		return m_selected == drawable;
	}
	void ResetSelected() {
		m_selected = nullptr;
	}
};

inline Inspector* GetInspector() { return Inspector::GetInstance(); }