#include "stdafx.h"

PlayerObject::PlayerObject(const String& name) : GameObject(name, true) {
	m_invincible = false;
}

EditorObjectType PlayerObject::GetObjectType() const {
	return EditorObjectType::PLAYER;
}

void PlayerObject::Update(const TimeStep& time) {
	// Movement
	m_physicsObject.m_velocity.y -= m_physicsObject.m_gravity * time;
	float decay = 1 / (1 + (time * m_physicsObject.m_friction));
	m_physicsObject.m_velocity.x *= decay;

	if (GetKeyboard()->KeyDown('A')) {
		GetAudioManager()->Play(GetAssetManager()->Get<Audio>("BloopSound"));
		m_physicsObject.m_velocity.x -= m_movementSpeed * time;
	}

	if (GetKeyboard()->KeyDown('D')) {
		GetAudioManager()->Play(GetAssetManager()->Get<Audio>("BloopSound"));
		m_physicsObject.m_velocity.x += m_movementSpeed * time;
	}

	if (m_physicsObject.m_isGrounded && GetKeyboard()->KeyJustDown(VK_SPACE)) {
		m_physicsObject.m_velocity.y = 45;
		m_physicsObject.m_isGrounded = false;
	}

	//Stops mid-air jump
	if (m_physicsObject.m_velocity.y < 0) {
		m_physicsObject.m_isGrounded = false;
	}

	// Invincibility
	if (m_invincible) {
		int colorTime = (int)Math::Floor(m_invincibilityTimer.Get(Timer::SECONDS) * 1000) / 10 % 2;
		SetColor(colorTime == 0 ? Color::Yellow() : Color::White());
		if ((m_invincibilityTimer.Get(Timer::SECONDS) * 10) >= 3) {
			SetInvinciblity(false);
			SetColor(Color::White());
		}
	}

	m_physicsObject.Update(time);
}

GameObject* PlayerObject::Copy() {
	return new PlayerObject(*this);
}

void PlayerObject::InspectorDraw() {
	GameObject::InspectorDraw();
}

PlayerObject* PlayerObject::SetMovementSpeed(float speed) {
	m_movementSpeed = speed;
	return this;
}

PlayerObject* PlayerObject::SetInvinciblity(bool invincible) {
	m_invincible = invincible;
	if (invincible) {
		m_invincibilityTimer = Timer();
	}
	return this;
}