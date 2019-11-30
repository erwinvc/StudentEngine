#pragma once

class Hierarchy {
public:
	vector<GameObject*> m_gameObjects;

	GameObject* AddGameObject(GameObject* gameObject) {
		m_gameObjects.push_back(gameObject);
		return gameObject;
	}

	void Draw(const RenderingPipeline* pipeline) {
		for (auto gObj : m_gameObjects) {
			gObj->Draw(pipeline);
		}
	}
};