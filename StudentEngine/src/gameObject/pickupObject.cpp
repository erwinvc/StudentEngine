#include "stdafx.h"

PickupObject::PickupObject(const String& name) : GameObject(name, true) {

}

EditorObjectType PickupObject::GetObjectType() {
	return EditorObjectType::PICKUP;
}

GameObject* PickupObject::Copy() {
	return new PickupObject(*this);
}

void PickupObject::InspectorDraw() {
	GameObject::InspectorDraw();
}

nlohmann::json PickupObject::ToJson() {
	nlohmann::json resultJson = GameObject::ToJson();
	return resultJson;
}

PickupObject* PickupObject::SetPickupType(PickupType type) {
	switch (type) {
	case PickupType::COIN:
		SetTexture(GetAssetManager()->Get<StreamedTexture>("BluePickup"));
		break;
	case PickupType::POWERUP:
		SetTexture(GetAssetManager()->Get<StreamedTexture>("GreenPickup"));
		break;
	}

	m_type = type;
	return this;
}

void PickupObject::OnPickup() {
	switch (m_type) {
	case PickupType::COIN:
		break;
	case PickupType::POWERUP:
		PlayerObject* playerObject = static_cast<PlayerObject*>(GetActiveScene()->FindObjectByName("Player"));
		playerObject->SetInvinciblity(true);
		break;
	}
}