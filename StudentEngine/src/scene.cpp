#include "stdafx.h"

void Scene::Update(const TimeStep& time) {
	Vector3 ray = GroundRaycast::GetMousePosition(GetCamera());
	m_cursorWorldPosition = GroundRaycast::GetGroundPosition(GetCamera(), ray, 1.0f);
	if (GetStateManager()->GetState() == States::PLAY) m_hierarchy.Update(time);
}
GameObject* Scene::GetGameObjectUnderMouse() {
	if (!GetEditorWindow()->IsMouseInViewport()) {
		return nullptr;
	}

	for (int i = (int)m_hierarchy.m_gameObjects.size() - 1; i >= 0; i--) {
		GameObject*& gObj = m_hierarchy.m_gameObjects[i];
		if (gObj->m_transform.IsPointWithin(m_cursorWorldPosition.x, m_cursorWorldPosition.y)) {
			return gObj;
		}
	}
	return nullptr;
}