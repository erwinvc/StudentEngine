#include "stdafx.h"

PlayerObject::PlayerObject(const String& name, float movementSpeed) : GameObject(name), m_movementSpeed(movementSpeed) {

}

void PlayerObject::Update(const TimeStep& time) {
	if (GetKeyboard()->KeyDown(GLFW_KEY_A)) {
		m_transform.m_position.x -= m_movementSpeed;
	}

	if (GetKeyboard()->KeyDown(GLFW_KEY_D)) {
		m_transform.m_position.x += m_movementSpeed;
	}
}