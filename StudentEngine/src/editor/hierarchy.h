
class Hierarchy {
private:
	GameObject* m_selected;

public:
	~Hierarchy() {
		Clear();
	}

	void Initialize() {
		m_layers.push_back(new ObjectLayer("Background"));
		m_layers.push_back(new ObjectLayer("Objects"));
		m_layers.push_back(new ObjectLayer("Pickups"));
		m_layers.push_back(new ObjectLayer("Foreground"));
	}

	vector<ObjectLayer*> m_layers;

	GameObject* AddGameObject(GameObject* gameObject) {
		FindLayerByName(gameObject->m_layer)->m_objects.push_back(gameObject);
		return gameObject;
	}

	ObjectLayer* FindLayerByName(String layer) {
		for (size_t i = 0; i < m_layers.size(); i++) {
			if (m_layers[i]->m_layerName == layer) {
				return m_layers[i];
			}
		}
		LOG("%s", "Couldn't find the layer by name!");
		return NULL;
	}

	void Update(const TimeStep& time) {
		for (auto layer : m_layers) {
			layer->Update(time);
		}
	}

	void Draw(RenderingPipeline* pipeline) {
		for (auto gObj : m_layers) {
			gObj->Draw(pipeline, m_selected);
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
		for (auto& layer : m_layers) {
			layer->Clear();
			delete layer;
		}
		m_layers.clear();
	}
	
	void DeleteGameObject(GameObject* obj) {
		GetInspector()->ResetSelected();
		m_selected = nullptr;

		//vector<GameObject*> objects = FindLayerByName(obj->m_layer)->m_objects;
		//direct ref with objects didnt work??? :thonk:
		FindLayerByName(obj->m_layer)->m_objects.erase(remove(FindLayerByName(obj->m_layer)->m_objects.begin(), FindLayerByName(obj->m_layer)->m_objects.end(), obj), FindLayerByName(obj->m_layer)->m_objects.end());
		delete obj;
	}

	void SetSelected(GameObject* selected) {
		m_selected = selected;
	}

	GameObject* GetSelected() {
		return m_selected;
	}
};