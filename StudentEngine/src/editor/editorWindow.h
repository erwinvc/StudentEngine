#pragma once


class editorWindow : public Singleton<editorWindow> {
private:
	float m_time = 0;
	bool inEditorMode = true;
	bool settingNewParent = false;
	bool draggingItem = false;
	int childObjIndex = -1;

	vector<GameObject*> objs;

	//RenderWindow
	ImGuiID m_dockspaceCenter;
	ImGuiID m_dockspaceLeft;
	ImGuiID m_dockspaceRight;
	ImGuiID m_dockspaceBottom;
	ImGuiID m_dockspaceLeftBottom;

	ImVec2 m_mainWindowPos;
	Vector4 m_viewport;
	void CreateDockingSpace();
	void CreateEditorWindows();
	void CreateTemporaryPlayMode();
	void CreateViewport();
	void CreateSceneOverview(ImGuiWindowFlags flags);

	void DisplaySceneChild(int index, bool hasChildren);
	void OnItemSelect(GameObject* obj);
	void OnItemDelete(int index);
	void OnItemRename(int index);
	void AddItem(Vector2 pos);
	void ToggleSettingNewParent();
	void SettingNewParent(int parent, int child);
	void GuiItemDrag();
	void SetupEditorStyle(bool styleDark, float alpha);

public:
	editorWindow() {}
	~editorWindow() {}
	friend Singleton;

	void Initialize();
	void OnImGui();
	void Draw();
	void Update(const TimeStep& time);

	Vector4 GetViewport() { return m_viewport; }
};

static editorWindow* GetEditorWindow() { return editorWindow::GetInstance(); }