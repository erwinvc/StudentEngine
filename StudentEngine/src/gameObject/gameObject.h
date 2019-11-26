#pragma once

class GameObject {
	String m_name;
	Transform m_transform;

	GameObject* m_parent;
	vector<GameObject*> m_children;

	void AddChild(GameObject* gameObject) {
		gameObject->SetParent(this);
		m_children.push_back(gameObject);
	}

	void SetParent(GameObject* gameObject) {
		m_parent = gameObject;
	}

	GameObject* GetParent() { return m_parent; }
};