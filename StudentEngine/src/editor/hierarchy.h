
class Hierarchy {
private:
	GameObject* m_selected;

public:
	~Hierarchy() {
		Clear();
	}

	vector<GameObject*> m_gameObjects;

	GameObject* AddGameObject(GameObject* gameObject) {
		m_gameObjects.push_back(gameObject);
		return gameObject;
	}

	void Update(const TimeStep& time) {
		for (auto gObj : m_gameObjects) {
			gObj->Update(time);
		}
	}

	void Draw(RenderingPipeline* pipeline) {
		for (auto gObj : m_gameObjects) {
			gObj->Draw(pipeline);
			if (gObj == m_selected) EditorGameObject::Draw(pipeline, m_selected);
		}
	}

	void EditorDraw(RenderingPipeline* pipeline) {
		if (!GetInspector()->IsSelected(m_selected)) m_selected = nullptr;
		if (m_selected) EditorGameObject::Draw(pipeline, m_selected);
	}

	bool UpdateSelected(const TimeStep& time, Vector2 mousePosition) {
		return m_selected ? EditorGameObject::Update(m_selected, time, mousePosition) : false;
	}

	void Clear() {
		for (auto& obj : m_gameObjects) {
			delete obj;
		}
		m_gameObjects.clear();
	}
	
	void DeleteGameObject(GameObject* obj) {
		m_gameObjects.erase(remove(m_gameObjects.begin(), m_gameObjects.end(), obj), m_gameObjects.end());
		delete obj;
	}

	void SetSelected(GameObject* selected) {
		m_selected = selected;
	}

	GameObject* GetSelected() {
		return m_selected;
	}
};