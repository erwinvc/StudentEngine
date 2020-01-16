#pragma once

class WalkingEnemy : public EnemyObject
{
private:
	int m_walkingDirection = 1;
	float m_movementSpeed = 0.1f;

	friend EnemyObject;
public:
	WalkingEnemy(const String& name);
	EditorObjectType GetObjectType() const override;
	GameObject* Copy() override;
	void Update(const TimeStep& time) override;
	void OnCollision(GameObject* self, GameObject* other, CollisionType type) override;

	void ToJson(nlohmann::json& j) const override {
		EnemyObject::ToJson(j);
	}

	void FromJson(const nlohmann::json& j) override {
		EnemyObject::FromJson(j);
	}
};