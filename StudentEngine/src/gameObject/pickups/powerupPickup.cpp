#include "stdafx.h"

PowerupPickup::PowerupPickup(const String& name) : PickupObject(name) {
	SetTexture(GetAssetManager()->Get<StreamedTexture>("GreenPickup"));
}

EditorObjectType PowerupPickup::GetObjectType() const {
	return EditorObjectType::PICKUP_POWERUP;
}

GameObject* PowerupPickup::Copy() {
	return new PowerupPickup(*this);
}

void PowerupPickup::OnPickup() {
	GetAudioManager()->Play(this, GetAssetManager()->Get<Audio>("PowerupPickupSound"));
	PlayerObject* playerObject = static_cast<PlayerObject*>(GetActiveScene()->FindObjectByName("Player"));
	playerObject->SetInvinciblity(true);
}