#include "stdafx.h"

void PhysicsObject::Update(const TimeStep& time) {
	if (!m_dynamic) return;
	Move(m_gameObject->m_transform);
}

bool PhysicsObject::CheckCollision(float xa, float ya) {
	for (auto& other : GetActiveScene()->GetHierarchy()) {
		if (m_gameObject->m_transform.CollidesWith(other, xa, ya)) {
			return true;
		}
	}
	return false;
}