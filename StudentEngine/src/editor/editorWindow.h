#pragma once

class EditorWindow {
private:
	float m_time = 0;
	bool m_inEditorMode = true;
	bool m_settingNewFolder = false;

	bool m_dragPlacement = false;
	bool m_draggingItem = false;
	bool m_dragSourceExternal = false;
	vector<HierarchyObject*> m_layers;

	ImGuiID m_dockspaceCenter;
	ImGuiID m_dockspaceLeft;
	ImGuiID m_dockspaceRight;
	ImGuiID m_dockspaceBottom;
	ImGuiID m_dockspaceLeftBottom;

	ImVec2 m_mainWindowPos;
	Vector4 m_viewport;
	bool m_mouseInViewport;

	void CreateDockingSpace();
	void CreateEditorWindows();
	void CreateTemporaryPlayMode();
	void CreateViewport();
	void CreateSceneOverview(ImGuiWindowFlags flags);

	void DisplayObject(GameObject* obj);
	void AddItem(Vector2 pos);
	void AddFolder();
	void CreateItemDrag();
	void SetupEditorStyle(bool styleDark, float alpha);

public:
	EditorWindow() { }
	~EditorWindow();
	GameObject* m_movingChild;

	void Initialize();
	void OnImGui();
	void Draw();
	void Update(const TimeStep& time);

	void MoveToFolder(HierarchyObject* folder, GameObject* movingChild);
	HierarchyObject* FindFolderOfObject(GameObject* obj);
	void ToggleSettingNewParent(GameObject* obj);
	bool SettingNewFolder() { return m_settingNewFolder; }
	void InstantiateDragging(bool externalSource);
	bool IsInDragPlacement() { return m_dragPlacement; }
	void SetDragPlacement(bool value) { m_dragPlacement = value; }
	
	
	
	//bool CurrentlyDragging() { return m_draggingItem; }
	//void SetCurrentlyDragging(bool value) { m_draggingItem = value; }

	bool IsMouseInViewport() {
		return m_mouseInViewport;
	}
	
	Vector4 GetViewport() { return m_viewport; }
};