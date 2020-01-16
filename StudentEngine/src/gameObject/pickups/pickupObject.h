#pragma once

class PickupObject : public GameObject {
private:
	bool m_collected = false;
public:
	PickupObject(const String& name);
	virtual EditorObjectType GetObjectType() const override;
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
	virtual void OnPickup() = 0;

	virtual void ToJson(nlohmann::json& j) const override {
		GameObject::ToJson(j);
	}
	virtual void FromJson(const nlohmann::json& j) override {
		GameObject::FromJson(j);
	}
};