#pragma once
class EditorWindow {
private:
	float m_time = 0;
	bool m_inEditorMode = true;
	bool m_settingNewFolder = false;

	bool m_dragPlacement = false;
	bool m_draggingItem = false;
	bool m_dragSourceExternal = false;

	bool m_openedLayerManager = false;
	bool m_openedInspector = false;

	bool m_openedRightClickMenu = false;
	ImVec2 m_rightClickPosition = ImVec2(0, 0);
	EditorObjectType m_currentlyDraggedEditorObjectType;
	vector<HierarchyObject*> m_layers;

	ImGuiID m_dockspaceCenter;
	ImGuiID m_dockspaceLeft;
	ImGuiID m_dockspaceRight;
	ImGuiID m_dockspaceBottom;
	ImGuiID m_dockspaceLeftBottom;
	ImGuiID m_dockspaceUp;

	ImVec2 m_mainWindowPos;
	Vector4 m_viewport;
	bool m_mouseInViewport;

	void CreateDockingSpace();
	void CreateEditorWindows();
	bool CreateMenuButton(const char* icon, bool activateCondition);
	void CreateTemporaryPlayMode();
	void CreateViewport();
	void CreateSceneOverview(ImGuiWindowFlags flags);

	void DisplayObject(GameObject* obj);
	void AddFolder();
	void CreateItemDrag(EditorObjectType type);
	void SetupEditorStyle(bool styleDark, float alpha);

public:
	EditorWindow() { }
	~EditorWindow();
	GameObject* m_movingChild;

	void Initialize();
	void ApplyLayers();
	void OnImGui();
	void OnItemTooltip(String text);
	void OnRightClickSelected();
	void Draw();
	void Update(const TimeStep& time);

	bool IsVIPObject(GameObject* obj);

	void OpenInspector() { m_openedInspector = true; }
	void MoveToFolder(HierarchyObject* folder, GameObject* movingChild);
	HierarchyObject* FindFolderOfObject(GameObject* obj);
	void ToggleSettingNewParent(GameObject* obj);
	bool SettingNewFolder() { return m_settingNewFolder; }
	void InstantiateDragging(bool externalSource);
	bool IsInDragPlacement() { return m_dragPlacement; }
	void SetDragPlacement(bool value) { m_dragPlacement = value; }
	void ToggleEditMode();
	
	//bool CurrentlyDragging() { return m_draggingItem; }
	//void SetCurrentlyDragging(bool value) { m_draggingItem = value; }

	bool IsMouseInViewport() {
		return m_mouseInViewport;
	}
	
	Vector4 GetViewport() { return m_viewport; }
};