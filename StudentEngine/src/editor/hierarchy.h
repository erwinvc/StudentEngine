
class Hierarchy {
private:
	GameObject* m_selected;

public:
	~Hierarchy() {
		Clear();
	}

	vector<GameObject*> m_gameObjects;

	template<typename T>
	T* AddGameObject(T* gameObject) {
		m_gameObjects.push_back(gameObject);
		return gameObject;
	}

	void Update(const TimeStep& time) {
		for (auto it = m_gameObjects.begin(); it != m_gameObjects.end();) {
			(*it)->Update(time);
			if ((*it)->m_destroyNextFrame) {
				it = m_gameObjects.erase(it);
			} else it++;
		}
	}

	void Draw(RenderingPipeline* pipeline) {
		for (auto gObj : m_gameObjects) {
			gObj->Draw(pipeline);
			if (gObj == m_selected) EditorGameObject::Draw(pipeline, m_selected);

			//for (auto oObj : m_gameObjects) {
			//	if (gObj != oObj && gObj->m_transform.CollidesWith(oObj, 0, 0)) {
			//		pipeline->LineRect(gObj->m_transform.AsRectangle(), Color::Cyan(), 0.5f);
			//	}
			//}
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

	GameObject* FindObjectByName(const String& name) {
		auto it = find_if(m_gameObjects.begin(), m_gameObjects.end(), [name](GameObject* s) { return s->m_name == name; });
		if (it != m_gameObjects.end()) return *it;
		return nullptr;
	}

	void SetSelected(GameObject* selected) {
		m_selected = selected;
	}

	int Size() {
		return m_gameObjects.size();
	}

	GameObject* GetSelected() {
		return m_selected;
	}

	vector<GameObject*>::iterator begin() { return m_gameObjects.begin(); }
	vector<GameObject*>::iterator end() { return m_gameObjects.end(); }
};