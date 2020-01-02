#include "stdafx.h"

void PhysicsObject::Update(const TimeStep& time) {
	if (!m_dynamic) return;
	Move(m_gameObject->m_transform);
}

bool PhysicsObject::CheckCollision(float xa, float ya) {
	for each (auto layer in GetActiveScene()->GetHierarchy().m_layers) {
		for each (auto other in layer->m_objects) {
			if (m_gameObject->m_transform.CollidesWith(other, xa, ya)) {
				if (other->OnCollision(m_gameObject)) return true;
			}
		}
	}
	return false;
}