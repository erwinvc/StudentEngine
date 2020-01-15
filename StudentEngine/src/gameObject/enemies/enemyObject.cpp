#include "stdafx.h"

EnemyObject::EnemyObject(const String& name) : GameObject(name, true) {
	SetSize(Vector2(64, 64));
	SetOnCollision([&](GameObject* self, GameObject* other, CollisionType type) {
		OnCollision(self, other, type);
		return false;
	});
	m_layer = "Objects";
}

EditorObjectType EnemyObject::GetObjectType() const {
	return EditorObjectType::GAMEOBJECT;
}

GameObject* EnemyObject::Copy() {
	return new GameObject(*this);
}

void EnemyObject::OnCollision(GameObject* self, GameObject* other, CollisionType type) {
	if (other->IsOfType<PlayerObject>()) {
		if (type == CollisionType::TOP) {
			self->Destroy();
		}
		else {
			// Show game over
			static_cast<PlayState*>(GetStateManager()->GetState())->Restart();
		}
	}
}

void EnemyObject::InspectorDraw() {
	GameObject::InspectorDraw();
}