#pragma once

class StaticEnemy : public EnemyObject
{
public:
	StaticEnemy(const String& name);
	EditorObjectType GetObjectType() const override;
	GameObject* Copy() override;
	void Update(const TimeStep& time) override;

	void ToJson(nlohmann::json& j) const override {
		EnemyObject::ToJson(j);
	}

	void FromJson(const nlohmann::json& j) override {
		EnemyObject::FromJson(j);
	}
};