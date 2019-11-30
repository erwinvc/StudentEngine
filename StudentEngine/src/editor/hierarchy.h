#pragma once

class Hierarchy {
public:
	vector<GameObject*> m_gameObjects;
	GameObject* m_selected;

	GameObject* AddGameObject(GameObject* gameObject) {
		m_gameObjects.push_back(gameObject);
		return gameObject;
	}

	void Draw(RenderingPipeline* pipeline, Color& editorColor) {
		for (auto gObj : m_gameObjects) {
			gObj->Draw(pipeline);
			if (gObj == m_selected) m_selected->EditorDraw(pipeline, editorColor);
		}
	}
};