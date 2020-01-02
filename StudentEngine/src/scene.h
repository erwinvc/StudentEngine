#pragma once

class Scene {
public:
	Hierarchy m_hierarchy;
	Quadtree m_quadtree;
	Vector2 m_cursorWorldPosition;

	GameObject& AddGameObject(GameObject* gameObject) {
		return *m_hierarchy.AddGameObject(gameObject);
	}

	inline Hierarchy& GetHierarchy() { return m_hierarchy; }

	void Update(const TimeStep& time);
	void Draw(RenderingPipeline* pipeline) {
		m_hierarchy.Draw(pipeline);
		m_hierarchy.EditorDraw(pipeline);
		m_quadtree.Draw(pipeline);
	}

	Vector2	GetCursorWorldPosition() {
		return m_cursorWorldPosition;
	}
	
	GameObject* GetGameObjectUnderMouse();

	Scene() {
		m_quadtree.Initialize(10000, 10000);
		m_hierarchy.Initialize();
	}
	~Scene() {
		m_hierarchy.Clear();
	}
};