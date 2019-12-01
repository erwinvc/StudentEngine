#pragma once

class GameObject {
public:
	String m_name;
	Transform m_transform;

	GameObject* m_parent = NULL;
	vector<GameObject*> m_children;
	GameObject(String name) {
		m_name = name;
	}

	void AddChild(GameObject* gameObject) {
		gameObject->SetParent(this);
		m_children.push_back(gameObject);
	}

	void SetParent(GameObject* gameObject) {
		m_parent = gameObject;
	}

	GameObject* GetParent() { return m_parent; }

	bool HasParent() { return m_parent != NULL; }
};