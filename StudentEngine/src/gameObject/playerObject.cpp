#include "stdafx.h"

PlayerObject::PlayerObject(const String& name) : GameObject(name, true) {

}

EditorObjectType PlayerObject::GetObjectType() {
	return EditorObjectType::PLAYER;
}

void PlayerObject::Update(const TimeStep& time) {
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
	
	m_physicsObject.Update(time);
}

GameObject* PlayerObject::Copy() {
	return new PlayerObject(*this);
}

void PlayerObject::InspectorDraw() {
	GameObject::InspectorDraw();
}

nlohmann::json PlayerObject::ToJson() {
	nlohmann::json resultJson = GameObject::ToJson();
	LOG("[~RSerialization~x] Serialized PlayerObject to JSON");
	return resultJson;
}

PlayerObject& PlayerObject::SetMovementSpeed(int speed) {
	m_movementSpeed = speed;
	return *this;
}