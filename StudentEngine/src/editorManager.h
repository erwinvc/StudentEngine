#pragma once

class EditorManager : public Singleton<EditorManager> {
public:
	void Draw();
};

inline EditorManager* GetEditorManager() { return EditorManager::GetInstance(); }