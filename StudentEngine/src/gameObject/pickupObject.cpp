#include "stdafx.h"

PickupObject::PickupObject(const String& name) : GameObject(name, true) {

}

map<const char*, PickupType> PickupObject::typeMap = {
	{ "BluePickup", PickupType::COIN }, { "GreenPickup", PickupType::POWERUP }
};

EditorObjectType PickupObject::GetObjectType() const {
	return EditorObjectType::PICKUP;
}

GameObject* PickupObject::Copy() {
	return new PickupObject(*this);
}

void PickupObject::InspectorDraw() {
	GameObject::InspectorDraw();
}

PickupObject* PickupObject::SetPickupType(PickupType type) {
	m_type = type;
	return this;
}

PickupType PickupObject::GetTypeFromTexture(const char* textureName) {
	return typeMap[textureName];
}

void PickupObject::OnPickup() {
	switch (m_type) {
	case PickupType::COIN:
		((PlayState*)GetStateManager()->GetState())->AdjustScore(5);
		break;
	case PickupType::POWERUP:
		PlayerObject* playerObject = static_cast<PlayerObject*>(GetActiveScene()->FindObjectByName("Player"));
		playerObject->SetInvinciblity(true);
		break;
	}
}