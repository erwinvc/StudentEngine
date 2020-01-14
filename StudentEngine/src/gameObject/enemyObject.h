#pragma once
enum class EnemyType {
	STATIC,
	WALKING
};

class EnemyObject : public GameObject
{
private:
	EnemyType m_type;
	static map<const char*, EnemyType> typeMap;

public:
	EnemyObject(const String& name);
	EditorObjectType GetObjectType() const override;
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
	EnemyObject* SetEnemyType(EnemyType type);
	static EnemyType GetTypeFromTexture(const char* textureName);

	void ToJson(nlohmann::json& j) const override {
		GameObject::ToJson(j);
		j["enemy type"] = m_type;
	}
	void FromJson(const nlohmann::json& j) override {
		GameObject::FromJson(j);
		j.at("enemy type").get_to(m_type);
	}
};