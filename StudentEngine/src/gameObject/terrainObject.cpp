#include "stdafx.h"

TerrainObject::TerrainObject(const String& name) : GameObject(name, false) {
	Set9Slice(true);
}

EditorObjectType TerrainObject::GetObjectType() const {
	return EditorObjectType::TERRAIN;
}

GameObject* TerrainObject::Copy() {
	return new TerrainObject(*this);
}

void TerrainObject::InspectorDraw() {
	GameObject::InspectorDraw();
}