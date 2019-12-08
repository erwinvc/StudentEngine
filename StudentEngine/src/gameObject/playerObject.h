#pragma once

class PlayerObject : public GameObject
{
private:
	float m_movementSpeed;
public:
	PlayerObject(const String& name, float movementSpeed);
	void Update(const TimeStep& time) override;

	virtual GameObject* Copy() override  {
		return new PlayerObject(*this);
	}
};