#pragma once

class GoalObject : public GameObject
{
public:
	GoalObject(const String& name);
	EditorObjectType GetObjectType() const override;
	virtual GameObject* Copy() override;

	void ToJson(nlohmann::json& j) const override {
		GameObject::ToJson(j);
	}
	void FromJson(const nlohmann::json& j) override {
		GameObject::FromJson(j);
	}
};

