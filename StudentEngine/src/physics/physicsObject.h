#pragma once

class GameObject;
class PhysicsObject {
private:
	GameObject* m_gameObject;
public:
	PhysicsObject(GameObject* gameObject) : m_gameObject(gameObject) {

	}

	GameObject* GetGameObject() {
		return m_gameObject;
	}
};