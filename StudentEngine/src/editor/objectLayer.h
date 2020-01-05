#pragma once

class ObjectLayer {
private:


public:
	vector<GameObject*> m_objects;
	String m_layerName;

	ObjectLayer(String name) {
		m_layerName = name;
	}

	~ObjectLayer() {
		Clear();
	}
	
	void AddGameObject(GameObject* obj, String layer);
	GameObject* FindObjectByName(const String& name);
	void Update(const TimeStep& time);
	void Draw(RenderingPipeline* pipeline, GameObject* selected);
	void Clear();
};