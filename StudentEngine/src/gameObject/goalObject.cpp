#include "stdafx.h"

GoalObject::GoalObject(const String& name) {
	SetSize(Vector2(64, 64));
	SetTexture(GetAssetManager()->Get<StreamedTexture>("Goal"));
	SetOnCollision([](GameObject* self, GameObject* other, CollisionType type) {
		if (other->IsOfType<PlayerObject>()) {
			static_cast<PlayState*>(GetStateManager()->GetState())->Restart();
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