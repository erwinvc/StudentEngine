#pragma once
enum class EnemyType {
	STATIC,
	WALKING
};

class EnemyObject : public GameObject
{
private:
	EnemyType m_type;
	static map<string, EnemyType> typeMap;
	int m_walkingDirection = 1;
	float m_movementSpeed = 0.1;

public:
	EnemyObject(const String& name);
	EditorObjectType GetObjectType() const override;
	virtual GameObject* Copy() override;
	void Update(const TimeStep& time) override;
	void InspectorDraw() override;
	EnemyObject* SetEnemyType(EnemyType type);
	static EnemyType GetTypeFromTexture(string& textureName);

	void ToJson(nlohmann::json& j) const override {
		GameObject::ToJson(j);
		j["enemy type"] = m_type;
	}
	void FromJson(const nlohmann::json& j) override {
		GameObject::FromJson(j);
		j.at("enemy type").get_to(m_type);
	}
};