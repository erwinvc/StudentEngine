#include "stdafx.h"

PickupObject::PickupObject(const String& name) : GameObject(name, true) {
	SetSize(Vector2(32, 32));
	SetAtlasValues(4, 4, 0.125f);
	SetOnCollision([](GameObject* self, GameObject* other, CollisionType type) {
		if (other->IsOfType<PlayerObject>()) {
			PickupObject* pickupObject = static_cast<PickupObject*>(self);
			if (!pickupObject->m_collected) {
				pickupObject->Destroy();
				pickupObject->OnPickup();
				pickupObject->m_collected = true;
			}
		}
		return false;
		});
	m_layer = "Pickups";
	m_collected = false;
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