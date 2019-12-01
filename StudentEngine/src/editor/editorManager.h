#pragma once

class EditorManager : public Singleton<EditorManager> {
private:
	Hierarchy m_hierarchy;
	Vector2 m_mouseRayPosition;
	GameObject* m_sample;

	void UpdateSelected(const TimeStep& time);
	
public:
	static Texture* g_logo;
	static Texture* g_buttonGizmo;
	static Texture* g_arrowGizmo;
	static Texture* g_squareGizmo;
	
	GameObject& AddGameObject(GameObject* gameObject) {
		return *m_hierarchy.AddGameObject(gameObject);
	}
	void Initialize();
	void Update(const TimeStep& time);
	void Draw(RenderingPipeline* pipeline);

	inline Hierarchy& GetHierarchy() { return m_hierarchy; }
};

inline EditorManager* GetEditorManager() { return EditorManager::GetInstance(); }