#pragma once


class editorWindow : public Singleton<editorWindow> {
private:
	float m_time = 0;
	bool m_inEditorMode = true;
	bool m_settingNewFolder = false;
	//SET BACK TO METHODS AAA
	//bool m_draggingItem = false;
	GameObject* m_movingChild;
	vector<HierarchyObject*> m_folders;

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

	void DisplayFolder(int index, bool hasChildren);
	void DisplayObject(GameObject* obj);
	//void OnItemSelect(GameObject* obj);
	void OnItemDelete(int index);
	void OnItemRename(int index);
	void AddItem(Vector2 pos);
	void AddFolder();
	void CreateItemDrag();
	void SetupEditorStyle(bool styleDark, float alpha);

public:
	editorWindow() {}
	~editorWindow() {}
	friend Singleton;
	bool m_draggingItem = false;
	bool m_dragPlacement = false;

	void Initialize();
	void OnImGui();
	void Draw();
	void Update(const TimeStep& time);

	void MoveToFolder(HierarchyObject* folder);
	HierarchyObject* FindFolderOfObject(GameObject* obj);
	void ToggleSettingNewParent(GameObject* obj);
	bool SettingNewFolder() { return m_settingNewFolder; }
	
	
	
	//bool CurrentlyDragging() { return m_draggingItem; }
	//void SetCurrentlyDragging(bool value) { m_draggingItem = value; }

	Vector4 GetViewport() { return m_viewport; }
};

static editorWindow* GetEditorWindow() { return editorWindow::GetInstance(); }