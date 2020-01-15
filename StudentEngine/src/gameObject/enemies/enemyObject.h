#pragma once
class EnemyObject : public GameObject
{
public:
	EnemyObject(const String& name);
	EditorObjectType GetObjectType() const override;
	virtual GameObject* Copy() override;
	virtual void Update(const TimeStep& time) = 0;
	void InspectorDraw() override;

	void ToJson(nlohmann::json& j) const override {
		GameObject::ToJson(j);
	}

	void FromJson(const nlohmann::json& j) override {
		GameObject::FromJson(j);
	}
};