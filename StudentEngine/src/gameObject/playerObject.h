#pragma once

class PlayerObject : public GameObject {
private:
	float m_movementSpeed;
	bool m_invincible;
	Timer m_invincibilityTimer;
public:
	PlayerObject(const String& name);
	EditorObjectType GetObjectType() const override;
	void Update(const TimeStep& time) override;
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
	nlohmann::json ToJson() override;
	PlayerObject* SetMovementSpeed(float speed);
	PlayerObject* SetInvinciblity(bool invincible);
};