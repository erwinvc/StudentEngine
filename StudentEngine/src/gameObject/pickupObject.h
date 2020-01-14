#pragma once
enum class PickupType {
	COIN,
	POWERUP
};

class PickupObject : public GameObject {
private:
	PickupType m_type;
	static map<const char*, PickupType> typeMap;
public:
	PickupObject(const String& name);
	EditorObjectType GetObjectType() const override;
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
	PickupObject* SetPickupType(PickupType type);
	static PickupType GetTypeFromTexture(const char* textureName);
	void OnPickup();

	void ToJson(nlohmann::json& j) const override {
		GameObject::ToJson(j);
		j["pickup type"] = m_type;
	}
	void FromJson(const nlohmann::json& j) override {
		GameObject::FromJson(j);
		j.at("pickup type").get_to(m_type);
	}
};