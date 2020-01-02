#include "stdafx.h"

void ObjectLayer::AddGameObject(GameObject* obj, String layer) {

}


void ObjectLayer::Update(const TimeStep& time) {
	for (auto gObj : m_objects) {
		gObj->Update(time);
	}
}

void ObjectLayer::Draw(RenderingPipeline* pipeline, GameObject* selected) {
	for (auto gObj : m_objects) {
		if (gObj->m_destroyNextFrame) {
			m_objects.erase(remove(m_objects.begin(), m_objects.end(), gObj), m_objects.end());
			continue;
		}

		gObj->Draw(pipeline);
		if (gObj == selected) EditorGameObject::Draw(pipeline, selected);
	}
}

void ObjectLayer::Clear() {
	for (auto& obj : m_objects) {
		delete obj;
	}
	m_objects.clear();
}
