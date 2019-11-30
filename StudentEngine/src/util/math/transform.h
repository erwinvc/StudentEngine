#pragma once

class Transform {
public:
	Vector2 m_position;
	Vector2 m_size;
	float m_rotation;

	Transform() : m_position(Vector2()), m_size(Vector2()), m_rotation(0) {}

	float XMin() { return m_position.x - m_size.x / 2; }
	float XMax() { return m_position.x + m_size.x / 2; }
	float YMin() { return m_position.y - m_size.y / 2; }
	float YMax() { return m_position.y + m_size.y / 2; }

	bool IsPointWithin(float x, float y) {
		return Math::Within(x, XMin(), XMax()) && Math::Within(y, YMin(), YMax());
	}
};