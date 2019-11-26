#pragma once

class EditorManager : public Singleton<EditorManager> {
private:
	Hierarchy m_hierarchy;
public:
	void Initialize();
	void Draw();

	inline Hierarchy& GetHierarchy() { return m_hierarchy; }
};

inline EditorManager* GetEditorManager() { return EditorManager::GetInstance(); }