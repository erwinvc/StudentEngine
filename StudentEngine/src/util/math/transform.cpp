#include "stdafx.h"

bool Transform::CollidesWith(GameObject* other, float xOffset, float yOffset) {
	if (other == m_gameObject) return false;
	return (Math::Within(XMin() + xOffset, XMax() + xOffset, other->m_transform.XMin(), other->m_transform.XMax()) &&
		Math::Within(YMin() + yOffset, YMax() + yOffset, other->m_transform.YMin(), other->m_transform.YMax()));
}