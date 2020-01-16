#include "stdafx.h"

void Camera::Update(const TimeStep time) {
	if (m_target != nullptr) {
		m_position.x = m_target->m_transform.m_position.x - (m_viewPort.width * 0.5f);
		m_position.y = m_target->m_transform.m_position.y - (m_viewPort.height * 0.4f);

		/*Vector2 diff = (m_target->m_transform.m_position - (m_startDiff + Vector2(m_position.x, m_position.y)));
		//LOG("%f - %f", diff.x, diff.y);
		if (diff.y > m_deadzone.y || diff.y < -m_deadzone.y) {
			if (diff.y < 0)
				m_position.y += (diff.y + m_deadzone.y) * 0.1f;
			else
				m_position.y += (diff.y - m_deadzone.y) * 0.1f;
		}

		if (diff.x > m_deadzone.x || diff.x < -m_deadzone.x) {
			if (diff.x < 0)
				m_position.x += (diff.x + m_deadzone.x) * 0.1f;
			else
				m_position.x += (diff.x - m_deadzone.x) * 0.1f;
		}*/

	}
}

void Camera::SetTarget(GameObject* target) {
	m_target = target;
	m_position.x = target->m_transform.m_position.x - (m_viewPort.width * 0.5f);
	m_position.y = target->m_transform.m_position.y - (m_viewPort.height * 0.4f);
	//m_startDiff = Vector2(m_target->m_transform.m_position.x - m_position.x, m_target->m_transform.m_position.y - m_position.y);
}