#pragma once

class HierarchyObject {
private:
	ObjectLayer* m_layer;
	char m_renameHelper[32];

public:
	String m_name;
	HierarchyObject(ObjectLayer* layer) : m_layer(layer) {}
	HierarchyObject() : m_name("") {}

	//virtual void Update(const TimeStep& time);

	void OnImGui();
	void RightClick(GameObject* obj, bool folder);
	void DisplayChild(GameObject* obj);
	void GuiObjectDrag(int index);
	void FolderExtra();
	void OnItemSelect(GameObject* obj);
	void OnItemDelete(GameObject* obj);

	//void AddChild(GameObject* gameObject);
	//void RemoveChild(ObjectLayer* layer, GameObject* gameObject);
	vector<GameObject*> GetChildren();
	GameObject* GetChildAt(int index);
	bool ContainsChild(GameObject* child);

};