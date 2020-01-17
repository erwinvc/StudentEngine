#include "stdafx.h"

GoalObject::GoalObject(const String& name) : GameObject(name, false) {
	SetSize(Vector2(64, 64));
	SetTexture(GetAssetManager()->Get<StreamedTexture>("Goal"));
	SetOnCollision([](GameObject* self, GameObject* other, CollisionType type) {
		if (other->IsOfType<PlayerObject>()) {
			for (int i = 0; i < 200; i++)
				GetParticleSystem()->Add(self->m_transform.m_position.x, self->m_transform.m_position.y, Math::RandomFloat(-5, 5), Math::RandomFloat(-5, 5), 0.0f, Math::RandomFloat(15, 15), Math::RandomFloat(15, 15), 0.0f, Math::RandomFloat(1000, 1500), 0, Color::Mix(Color(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f), Color(255.0f / 255.0f, 165.0f / 255.0f, 0), Math::RandomFloat(0, 1)));

			static_cast<PlayState*>(GetStateManager()->GetState())->Victory();
		}
		return true;
		});
}

EditorObjectType GoalObject::GetObjectType() const {
	return EditorObjectType::GOAL;
}

GameObject* GoalObject::Copy() {
	return new GoalObject(*this);
}