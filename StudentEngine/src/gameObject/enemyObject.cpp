#include "stdafx.h"

EnemyObject::EnemyObject(const String& name) : GameObject(name, true) {

}

map<const char*, EnemyType> EnemyObject::typeMap = {
	{ "RedCat", EnemyType::STATIC }, { "PurpleCat", EnemyType::WALKING }
};

EditorObjectType EnemyObject::GetObjectType() const {
	return EditorObjectType::ENEMY;
}

GameObject* EnemyObject::Copy() {
	return new EnemyObject(*this);
}

void EnemyObject::InspectorDraw() {
	GameObject::InspectorDraw();
}

EnemyObject* EnemyObject::SetEnemyType(EnemyType type) {
	m_type = type;
	return this;
}

EnemyType EnemyObject::GetTypeFromTexture(const char* textureName) {
	return typeMap[textureName];
}