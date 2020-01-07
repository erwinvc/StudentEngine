#pragma once

class PlayerObject : public GameObject {
private:
	float m_movementSpeed = 0.5;
public:
	PlayerObject(const String& name);
	EditorObjectType GetObjectType();
	void Update(const TimeStep& time) override;
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
	nlohmann::json ToJson() override;
	PlayerObject& SetMovementSpeed(int speed);
};