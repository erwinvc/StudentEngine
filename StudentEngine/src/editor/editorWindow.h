#pragma once

class editorWindow : public Singleton<editorWindow> {
private:
	float m_time = 0;
	bool inEditorMode = true;


	//RenderWindow
	ImGuiID m_dockspaceCenter;
	ImGuiID m_dockspaceLeft;
	ImGuiID m_dockspaceRight;
	ImGuiID m_dockspaceBottom;
	ImGuiID m_dockspaceLeftBottom;

	void CreateDockingSpace();
	void CreateEditorWindows();
	void CreateTemporaryPlayMode();
	void CreateViewport();
	//void RenderGUI();

public:
	editorWindow() {}
	~editorWindow() {}
	friend Singleton;

	void Initialize();
	void OnImGui();
	//void Begin();
	void End();
};

static editorWindow* GetEditorWindow() { return editorWindow::GetInstance(); }