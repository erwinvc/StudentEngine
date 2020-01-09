#include "stdafx.h"

void to_json(nlohmann::json& jsonObject, const Hierarchy& hierarchy) {
	for (ObjectLayer* layer : hierarchy.m_layers) {
		for (GameObject* gameObject : layer->m_objects) {
			jsonObject[gameObject->m_name] = *gameObject;
		}
	}
}

void from_json(const nlohmann::json& jsonObject, Hierarchy& hierarchy) {
	GetEditorScene()->GetHierarchy().Clear();
	GetEditorScene()->GetHierarchy().Initialize();
	nlohmann::json iteratedJson = jsonObject;
	for (nlohmann::json::iterator it = iteratedJson.begin(); it != iteratedJson.end(); ++it) {
		jsonObject.at(it.key()).get<GameObject>();
	}
	Hierarchy& myHierarchy = GetEditorScene()->GetHierarchy();
}