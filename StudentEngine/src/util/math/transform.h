#pragma once

class GameObject;
class Transform {
private:
	GameObject* m_gameObject;
	friend GameObject;
public:
	Vector2 m_position;
	Vector2 m_size;
	float m_rotation;

	Transform() : m_gameObject(nullptr), m_position(Vector2()), m_size(Vector2()), m_rotation(0) {}
	Transform(GameObject* gameObject) : m_gameObject(gameObject), m_position(Vector2()), m_size(Vector2()), m_rotation(0) {}
	Transform(const Transform& other, GameObject* gameObject) : m_gameObject(gameObject), m_position(other.m_position), m_size(other.m_size), m_rotation(other.m_rotation) {}

	//Transform(const Transform& other) = delete;

	float XMin() { return m_position.x - m_size.x / 2; }
	float XMax() { return m_position.x + m_size.x / 2; }
	float YMin() { return m_position.y - m_size.y / 2; }
	float YMax() { return m_position.y + m_size.y / 2; }

	Rectangle AsRectangle() {
		return { m_position, m_size };
	}

	bool IsPointWithin(float x, float y) {
		return Math::Within(x, XMin(), XMax()) && Math::Within(y, YMin(), YMax());
	}

	bool CollidesWith(GameObject* other, float xOffset, float yOffset);

	void InspectorDraw() {
		InspectorDrawer::Vec2("Position", m_position);
		InspectorDrawer::Vec2("Size", m_size);
	}
	GameObject* GetGameObject() {
		return m_gameObject;
	}
};