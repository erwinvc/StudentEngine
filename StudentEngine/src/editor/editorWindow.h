#pragma once


class editorWindow : public Singleton<editorWindow> {
private:
	float m_time = 0;
	bool inEditorMode = true;
	bool settingNewParent = false;
	int childObjIndex = -1;

	vector<GameObject*> objs;

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
	void CreateSceneOverview(ImGuiWindowFlags flags);


	void DisplaySceneChild(int index, bool hasChildren);
	void OnItemSelect(GameObject* obj);
	void OnItemDelete(int index);
	void OnItemRename(int index);
	void ToggleSettingNewParent();
	void SettingNewParent(int parent, int child);
	void AddItem();
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