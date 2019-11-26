#pragma once

class EditorManager : public Singleton<EditorManager> {
private:
	vector<GameObject> m_hierarchy;
public:
	void Initialize();
	void Draw();
};

inline EditorManager* GetEditorManager() { return EditorManager::GetInstance(); }