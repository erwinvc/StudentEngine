#include "stdafx.h"

void Camera::Update(const TimeStep time) {
	if (m_target != nullptr) {
		auto transform = m_target->m_transform;
		m_position.x = transform.m_position.x - (m_viewPort.width * 0.5f);
		//m_position.y = transform.m_position.y - (m_viewPort.height * 0.5f);
	}
}