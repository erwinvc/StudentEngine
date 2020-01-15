#include "stdafx.h"

TerrainObject::TerrainObject(const String& name) : GameObject(name, false) {
	SetSize(Vector2(64, 64));
	Set9Slice(true);
	SetTexture(GetAssetManager()->Get<StreamedTexture>("9slice"));
	m_layer = "Background";
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