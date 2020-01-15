#include "stdafx.h"

StaticEnemy::StaticEnemy(const String& name) : EnemyObject(name) {
	SetTexture(GetAssetManager()->Get<StreamedTexture>("RedCat"));
}

EditorObjectType StaticEnemy::GetObjectType() const {
	return EditorObjectType::ENEMY_STATIC;
}

GameObject* StaticEnemy::Copy() {
	return new StaticEnemy(*this);
}

void StaticEnemy::Update(const TimeStep& time) {
	m_physicsObject.m_velocity.y -= m_physicsObject.m_gravity * time;
	m_physicsObject.Update(time);
}