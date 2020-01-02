#include "stdafx.h"

PlayerObject::PlayerObject(const String& name, float movementSpeed) : GameObject(name, true), m_movementSpeed(movementSpeed) {

}

void PlayerObject::Update(const TimeStep& time) {
	m_physicsObject.m_velocity.y -= m_physicsObject.m_gravity * time;
	float decay = 1 / (1 + (time * m_physicsObject.m_friction));
	m_physicsObject.m_velocity *= decay;
	
	if (GetKeyboard()->KeyDown('A')) {
		m_physicsObject.m_velocity.x -= m_movementSpeed * time;
	}

	if (GetKeyboard()->KeyDown('D')) {
		m_physicsObject.m_velocity.x += m_movementSpeed * time;
	}

	if (m_physicsObject.m_isGrounded && GetKeyboard()->KeyJustDown(VK_SPACE)) {
		m_physicsObject.m_velocity.y += 100;
		m_physicsObject.m_isGrounded = false;
	}
	
	m_physicsObject.Update(time);
}