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
	for (int i = (int)m_hierarchy.m_layers.size() - 1; i >= 0; i--) {
		ObjectLayer* layer = m_hierarchy.m_layers[i];
		for (size_t j = 0; j < layer->m_objects.size(); j++) {
			if (layer->m_objects[j]->m_transform.IsPointWithin(m_cursorWorldPosition.x, m_cursorWorldPosition.y)) {
				return layer->m_objects[j];
			}
		}
	}
	return nullptr;
}