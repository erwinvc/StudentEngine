#pragma once

class InspectorDrawer {
public:
	static bool Header(String name) { return ImGui::CollapsingHeader(name.c_str()); }
	static bool Header(String_t name) { return ImGui::CollapsingHeader(name); }
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
	void ResetSelected() {
		m_selected = nullptr;
	}
};

inline Inspector* GetInspector() { return Inspector::GetInstance(); }