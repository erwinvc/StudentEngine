#include "stdafx.h"

void Keyboard::Initialize(Window* window) {
	for (int i = 0; i < KEYSIZE; i++) {
		m_keyStates[i] = Key();
	}

	GetGLCallbackManager()->AddOnKeyCallback(this, &Keyboard::OnKey);
	LOG("[~cInput~x] Initialized Keyboard");
}

void Keyboard::OnKey(int key, int scancode, int action, int mods) {
	if ((int)key < KEYSIZE) {
		if (action == GLFW_RELEASE) {
			if ((int)key < KEYSIZE) {
				GetKeyboard()->m_queue.Emplace(key, GetKeyboard()->m_keyStates[key].m_isUpNow, true);
			}
		} else if (action == GLFW_PRESS) {
			if ((int)key < KEYSIZE) {
				GetKeyboard()->m_queue.Emplace(key, GetKeyboard()->m_keyStates[key].m_isUpNow, false);
			}
		}
		if (action == GLFW_PRESS) GetKeyboard()->m_lastKey = key;
	}
}

void Keyboard::Update() {
	for (int i = 0; i < KEYSIZE; i++) {
		m_keyStates[i].m_wasDownBefore = !m_keyStates[i].m_isUpNow;
	}

	while (m_queue.Size() > 0) {
		KeyboardMessage message;
		if (m_queue.TryToGet(message)) {
			if (message.m_key < 0 || message.m_key > KEYSIZE) continue;
			m_keyStates[message.m_key].m_isUpNow = message.m_isUpNow;
			if (message.m_isUpNow) m_lastKey = message.m_key;
		}
	}
}

bool Keyboard::KeyDown(DWORD key) {
	return ((int)key < KEYSIZE) && !m_keyStates[key].m_isUpNow;
}

bool Keyboard::KeyJustUp(DWORD key) {
	return ((int)key < KEYSIZE) && m_keyStates[key].m_isUpNow && m_keyStates[key].m_wasDownBefore;
}

bool Keyboard::KeyJustDown(DWORD key) {
	return ((int)key < KEYSIZE) && !m_keyStates[key].m_isUpNow && !m_keyStates[key].m_wasDownBefore;
}