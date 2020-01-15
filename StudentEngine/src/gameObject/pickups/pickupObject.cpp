#include "stdafx.h"

PickupObject::PickupObject(const String& name) : GameObject(name, true) {
	SetSize(Vector2(32, 32));
	SetAtlasValues(4, 4, 0.125f);
	SetOnCollision([](GameObject* self, GameObject* other, CollisionType type) {
		if (other->IsOfType<PlayerObject>()) {
			self->Destroy();
			static_cast<PickupObject*>(self)->OnPickup();
		}
		return false;
		});
	m_layer = "Pickups";
}

EditorObjectType PickupObject::GetObjectType() const {
	return EditorObjectType::GAMEOBJECT;
}

GameObject* PickupObject::Copy() {
	return new GameObject(*this);
}

void PickupObject::InspectorDraw() {
	GameObject::InspectorDraw();
}