#include "stdafx.h"

EnemyObject::EnemyObject(const String& name) : GameObject(name, true) {
	SetSize(Vector2(32, 32));
	SetOnCollision([](GameObject* self, GameObject* other, CollisionType type) {
		if (other->IsOfType<TerrainObject>()) {
			if (self->m_physicsObject.m_velocity.x > 0) {
				if (self->m_transform.XMax() > other->m_transform.XMax()) {
					((WalkingEnemy*)self)->m_walkingDirection *= -1;
				}
			}
			if (self->m_physicsObject.m_velocity.x < 0) {
				if (self->m_transform.XMin() < other->m_transform.XMin()) {
					((WalkingEnemy*)self)->m_walkingDirection *= -1;
				}
			}
		}
		if (other->IsOfType<PlayerObject>()) {
			if (type == CollisionType::TOP) {
				self->Destroy();
			} else {
				// Show game over
				static_cast<PlayState*>(GetStateManager()->GetState())->Restart();
			}
			return true;
		}
	});
	m_layer = "Objects";
}

EditorObjectType EnemyObject::GetObjectType() const {
	return EditorObjectType::GAMEOBJECT;
}

GameObject* EnemyObject::Copy() {
	return new GameObject(*this);
}

void EnemyObject::InspectorDraw() {
	GameObject::InspectorDraw();
}