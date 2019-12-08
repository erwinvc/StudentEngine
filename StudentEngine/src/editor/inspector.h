#pragma once

class Inspector : public Singleton<Inspector> {
public:
	void OnImGui() {
		if (ImGui::Begin("Inspector", nullptr)) {
		}
		ImGui::End();
	}
};

inline Inspector* GetInspector() { return Inspector::GetInstance(); }