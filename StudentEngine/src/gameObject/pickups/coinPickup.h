#pragma once

class CoinPickup : public PickupObject
{
public:
	bool m_collected = false;
	CoinPickup(const String& name);
	EditorObjectType GetObjectType() const override;
	GameObject* Copy() override;
	void OnPickup() override;

	void ToJson(nlohmann::json& j) const override {
		PickupObject::ToJson(j);
	}

	void FromJson(const nlohmann::json& j) override {
		PickupObject::FromJson(j);
	}
};

