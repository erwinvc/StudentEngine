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
		if (type == CollisionType::TOP || static_cast<PlayerObject*>(other)->GetInvinciblity() == true) {
			self->Destroy();
			for (int i = 0; i < 10; i++)
				GetParticleSystem()->Add(self->m_transform.m_position.x + Math::RandomFloat(-5, 5), self->m_transform.m_position.y + Math::RandomFloat(-5, 5), Math::RandomFloat(-50, 50), Math::RandomFloat(-1, 5), 0.4f, Math::RandomFloat(16, 26), Math::RandomFloat(16, 26), 0.1f, Math::RandomFloat(250, 350), 4, Color::Mix(Color::Red(), Color(1, 0.3f, 0.3f), Math::RandomFloat(0.0f, 1.0f)));
		} else {
			// Show game over
			for (int i = 0; i < 10; i++)
				GetParticleSystem()->Add(other->m_transform.m_position.x + Math::RandomFloat(-5, 5), other->m_transform.m_position.y + Math::RandomFloat(-5, 5), Math::RandomFloat(-50, 50), Math::RandomFloat(-1, 5), 0.4f, Math::RandomFloat(16, 26), Math::RandomFloat(16, 26), 0.1f, Math::RandomFloat(250, 350), 4, Color::Mix(Color::Red(), Color(1, 0.3f, 0.3f), Math::RandomFloat(0.0f, 1.0f)));

			static_cast<PlayState*>(GetStateManager()->GetState())->GameOver();
		}
	}
}

void EnemyObject::InspectorDraw() {
	GameObject::InspectorDraw();
}