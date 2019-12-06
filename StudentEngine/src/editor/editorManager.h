#pragma once

class EditorManager : public Singleton<EditorManager> {
private:
	Hierarchy m_hierarchy;
	Vector2 m_mouseRayPosition;
	GameObject* m_sample;

	void EditorControls(const TimeStep& time);
	
public:
	static StreamedTexture* g_logo;
	static StreamedTexture* g_buttonGizmo;
	static StreamedTexture* g_arrowGizmo;
	static StreamedTexture* g_squareGizmo;
	
	GameObject& AddGameObject(GameObject* gameObject) {
		return *m_hierarchy.AddGameObject(gameObject);
	}
	void Initialize();
	void Update(const TimeStep& time);
	void Draw(RenderingPipeline* pipeline);
	void PostDraw(RenderingPipeline* pipeline);

	inline Hierarchy& GetHierarchy() { return m_hierarchy; }
};

inline EditorManager* GetEditorManager() { return EditorManager::GetInstance(); }