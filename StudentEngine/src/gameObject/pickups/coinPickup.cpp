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
	GetAudioManager()->Play(this, GetAssetManager()->Get<Audio>("CoinPickupSound"));

	GameObject* player = GetActiveScene()->FindObjectByName("Player");
	if (player) {
		GetParticleSystem()->Add(player->m_transform.m_position.x, player->m_transform.m_position.y + player->m_transform.m_size.y / 2, 0, 10, 0.0f, Math::RandomFloat(25, 25), Math::RandomFloat(25, 25), 0.01f, Math::RandomFloat(500, 550), 0, Color::Mix(Color(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f), Color(255.0f / 255.0f, 165.0f / 255.0f, 0), Math::RandomFloat(0, 1)));
	}
	static_cast<PlayState*>(GetStateManager()->GetState())->AdjustScore(5);
}