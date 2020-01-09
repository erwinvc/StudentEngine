#pragma once
class TerrainObject : public GameObject
{
public:
	TerrainObject(const String& name);
	EditorObjectType GetObjectType() const override;
	virtual GameObject* Copy() override;
	void InspectorDraw() override;
};

