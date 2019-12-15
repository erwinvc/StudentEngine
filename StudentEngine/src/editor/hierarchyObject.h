#pragma once

class HierarchyObject {
private:
	vector<GameObject*> m_children;
	char m_renameHelper[32];

public:
	String m_name;
	HierarchyObject(const String& name) : m_name(name) {}
	HierarchyObject() : m_name("") {}

	//virtual void Update(const TimeStep& time);

	void OnImGui();
	void RightClick(int index, bool folder);
	void DisplayChild(int index);
	void GuiObjectDrag(int index);
	void FolderExtra();
	void OnItemSelect(GameObject* obj);
	void OnItemDelete(int index);

	void AddChild(GameObject* gameObject);
	void RemoveChild(GameObject* gameObject);
	vector<GameObject*> GetChildren();
	GameObject* GetChildAt(int index);
	bool ContainsChild(GameObject* child);

};