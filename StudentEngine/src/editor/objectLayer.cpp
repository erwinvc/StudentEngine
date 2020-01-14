#include "stdafx.h"

void ObjectLayer::AddGameObject(GameObject* obj, String layer) {

}

void ObjectLayer::RemoveObject(GameObject * obj) {
	if (FindObjectByName(obj->m_name)) {
		m_objects.erase(remove(m_objects.begin(), m_objects.end(), obj), m_objects.end());
	} else {
		LOG("_ERROR Couldn't find given objec to delete in this layer!");
	}
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

GameObject* ObjectLayer::FindObjectByName(const String& name) {
	for (int i = 0; i < m_objects.size(); i++) {
		if (m_objects[i]->m_name == name) {
			return m_objects[i];
		}
	}

	return nullptr;
}


void ObjectLayer::Clear() {
	for (auto& obj : m_objects) {
		delete obj;
	}
	m_objects.clear();
}
