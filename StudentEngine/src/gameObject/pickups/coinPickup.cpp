#include "stdafx.h"

CoinPickup::CoinPickup(const String& name) : PickupObject(name) {
	SetTexture(GetAssetManager()->Get<StreamedTexture>("BluePickup"));
}

EditorObjectType CoinPickup::GetObjectType() const {
	return EditorObjectType::PICKUP_COIN;
}

GameObject* CoinPickup::Copy() {
	return new CoinPickup(*this);
}

void CoinPickup::OnPickup() {
	static_cast<PlayState*>(GetStateManager()->GetState())->AdjustScore(5);
}