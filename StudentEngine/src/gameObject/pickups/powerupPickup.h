#pragma once

class PowerupPickup : public PickupObject
{
public:
	PowerupPickup(const String& name);
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

