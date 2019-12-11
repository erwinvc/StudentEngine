#pragma once

class Window;

class ImGuiManager : public Singleton<ImGuiManager> {
private:
	float m_time = 0;
public:
	static ImFont* g_bigFont;
	static ImFont* g_smallFont;
	ImGuiManager() {}
	~ImGuiManager() {}
	friend Singleton;

	void Initialize(Window* window);
	void Begin();
	void End();
	void Cleanup();
};

namespace ImGui
{
	bool NamedButton(Texture* texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, bool selected, const char* label, bool double_click);
}
static ImGuiManager* GetImGuiManager() { return ImGuiManager::GetInstance(); }