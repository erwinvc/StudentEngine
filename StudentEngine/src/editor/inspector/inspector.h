#pragma once

class StreamedTexture;

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
	static void Text(String_t name, String_t text);
	static bool EditText(String_t name, String& text);
	static bool Vec2(String_t name, Vector2& vec);
	static void Color(String_t name, ::Color& color);
	static bool Combo(String_t name, int* item, String_t const items[], int itemCount);
	static void Texture(String_t name, StreamedTexture* texture);
	
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