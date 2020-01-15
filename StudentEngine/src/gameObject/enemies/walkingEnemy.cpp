#include "stdafx.h"

WalkingEnemy::WalkingEnemy(const String& name) : EnemyObject(name) {
	SetTexture(GetAssetManager()->Get<StreamedTexture>("PurpleCat"));
}

EditorObjectType WalkingEnemy::GetObjectType() const {
	return EditorObjectType::ENEMY_WALKING;
}

GameObject* WalkingEnemy::Copy() {
	return new WalkingEnemy(*this);
}

void WalkingEnemy::Update(const TimeStep& time) {
	m_physicsObject.m_velocity.y -= m_physicsObject.m_gravity * time;
	float decay = 1 / (1 + (time * m_physicsObject.m_friction));
	m_physicsObject.m_velocity.x *= decay;
	float xVelocity = m_walkingDirection * m_movementSpeed * time;
	float xOffset = m_transform.m_position.x - xVelocity;
	float yOffset = m_transform.m_position.y - m_sprite.m_texture->GetTexture()->GetHeight() - 1;
	//vector<GameObject*> backgroundObjects = GetActiveScene()->GetHierarchy().FindLayerByName("Background")->m_objects;
	//LOG("%d", backgroundObjects.size());
	//for (GameObject* other : GetActiveScene()->GetHierarchy().FindLayerByName("Background")->m_objects) {
	//	if (!m_transform.CollidesWith(other, xOffset, yOffset)) {
	//		m_walkingDirection *= -1;
	//	}
	//}
	m_physicsObject.m_velocity.x -= xVelocity;
	m_physicsObject.Update(time);
}

void WalkingEnemy::OnCollision(GameObject* self, GameObject* other, CollisionType type) {
	EnemyObject::OnCollision(self, other, type);
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
}