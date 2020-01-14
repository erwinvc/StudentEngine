#pragma once

class Scene {
public:
	Hierarchy m_hierarchy;
	//Quadtree m_quadtree;
	Vector2 m_cursorWorldPosition;

	template<typename T>
	T* AddGameObject(T* gameObject) {
		return (T*)m_hierarchy.AddGameObject(gameObject);
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
		return m_hierarchy.FindObjectByName(name, true);
	}

	Vector2	GetCursorWorldPosition() {
		return m_cursorWorldPosition;
	}

	GameObject* GetGameObjectUnderMouse();

	void Clear() {
		m_hierarchy.Clear();
	}

	void Copy(Scene* scene) {
		for (int l = 0; l < scene->GetHierarchy().m_layers.size(); l++) {
			auto objects = scene->GetHierarchy().m_layers[l]->m_objects;
			for (int i = 0; i < objects.size(); i++) {
				AddGameObject(objects[i]->Copy());
			}
		}
	}

	Scene() {
		//m_quadtree.Initialize(10000, 10000);
		m_hierarchy.Initialize();
	}
	~Scene() {
		m_hierarchy.Clear();
	}
};