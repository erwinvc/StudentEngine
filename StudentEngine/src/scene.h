#pragma once

class Scene {
public:
	Hierarchy m_hierarchy;
	//Quadtree m_quadtree;
	Vector2 m_cursorWorldPosition;

	template<typename T>
	T& AddGameObject(T* gameObject) {
		return *(T*)m_hierarchy.AddGameObject(gameObject);
	}

	inline Hierarchy& GetHierarchy() { return m_hierarchy; }

	void Update(const TimeStep& time);
	void Draw(RenderingPipeline* pipeline) {
		m_hierarchy.Draw(pipeline);
		m_hierarchy.EditorDraw(pipeline);
		//m_quadtree.Draw(pipeline);
	}

	void DeleteGameObject(GameObject* obj) {
		m_hierarchy.DeleteGameObject(obj);
	}
	
	GameObject* FindObjectByName(const String& name) {
		return m_hierarchy.FindObjectByName(name);
	}

	Vector2	GetCursorWorldPosition() {
		return m_cursorWorldPosition;
	}

	GameObject* GetGameObjectUnderMouse();

	void Clear() {
		m_hierarchy.Clear();
	}

	void Copy(Scene* scene) {
		for (int i = 0; i < scene->GetHierarchy().m_gameObjects.size(); i++) {
			AddGameObject(scene->GetHierarchy().m_gameObjects[i]->Copy());
		}
	}

	Scene() {
		//m_quadtree.Initialize(10000, 10000);
	}
	~Scene() {
		m_hierarchy.Clear();
	}
};