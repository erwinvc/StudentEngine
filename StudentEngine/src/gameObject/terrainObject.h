#pragma once
class TerrainObject : public GameObject
{
public:
	TerrainObject(const String& name);
	EditorObjectType GetObjectType();
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
	nlohmann::json ToJson() override;
};

