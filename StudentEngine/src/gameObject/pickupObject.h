#pragma once
enum class PickupType {
	COIN,
	POWERUP
};

class PickupObject : public GameObject
{
private:
	PickupType m_type;
public:
	PickupObject(const String& name);
	EditorObjectType GetObjectType();
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
	nlohmann::json ToJson() override;
	GameObject& SetPickupType(PickupType& type);
	void OnPickup();
};