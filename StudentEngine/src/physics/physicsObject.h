#pragma once

class GameObject;
class PhysicsObject {
private:
	float PRECISION = 1.0f;

public:
	GameObject* m_gameObject;
	Vector2 m_velocity;
	float m_gravity = 0.125f;
	float m_friction = 0.125f;
	bool m_dynamic;
	bool m_isGrounded = false;

	PhysicsObject() : m_gameObject(nullptr), m_dynamic(false), m_velocity({ 0 }) {}
	PhysicsObject(GameObject* gameObject, bool dynamic) : m_gameObject(gameObject), m_dynamic(dynamic), m_velocity({ 0 }) {}
	PhysicsObject(const PhysicsObject& other, GameObject* gameObject) : m_gameObject(gameObject), m_dynamic(other.m_dynamic), m_velocity(other.m_velocity) {
	}

	GameObject* GetGameObject() {
		return m_gameObject;
	}

	void Update(const TimeStep& time);

	bool CheckCollision(float xa, float ya);

	void Move(Transform& transform) {
		bool falling = m_velocity.y < 0;
		bool collisionX = MoveDirection(transform.m_position.x, m_velocity.x, Vector3(1, 0, 0));
		bool collisionY = MoveDirection(transform.m_position.y, m_velocity.y, Vector3(0, 0, 1));
		if (collisionX) m_velocity.x = 0;
		if (collisionY) {
			m_velocity.y = 0;
			if (falling) m_isGrounded = true;
		}
	}

	CollisionType GetCollisionType(float xa, float ya) {
		if (xa < 0) return LEFT;
		if (xa > 0) return RIGHT;
		if (ya < 0) return TOP;
		if (ya > 0) return BOTTOM;
	}

	bool MoveDirection(float& value, float delta, const Vector3& axis) {
		bool contact = false;
		while (delta != 0) {
			float precision = Math::Sign(delta, PRECISION);
			if (Math::Abs(delta) > PRECISION) {
				contact = CheckCollision(precision * axis.x, precision * axis.z);
				if (!contact) value += precision;
				else return contact;
				delta -= precision;
			} else {
				contact = CheckCollision(delta * axis.x, delta * axis.z);
				if (!contact) value += delta;
				else return contact;
				delta = 0;
			}
		}
		return false;
	}
};