#include "stdafx.h"

map<const char*, vector<const char*>> GameObject::m_validTextures = {};

void to_json(nlohmann::json& j, const GameObject& obj) {
	obj.ToJson(j);
}

void from_json(const nlohmann::json& j, GameObject& obj) {
	obj.FromJson(j);
}

void GameObject::ToJson(nlohmann::json& j) const {
	j = nlohmann::json{
		{ "name", m_name },
		{ "type", GetObjectType() },
		{ "sprite", m_sprite },
		{ "transform", m_transform }
	};

	if (m_parent) {
		j["parent"] = m_parent->m_name;
	}
}

void GameObject::FromJson(const nlohmann::json& j) {
	string objectName = j.at("name").get<string>();
	int objectType = j.at("type").get<int>();

	GameObject* newObject = ObjectFactory::CreateObject((EditorObjectType)objectType, objectName);

	j.at("sprite").get_to(newObject->m_sprite);
	j.at("transform").get_to(newObject->m_transform);

	if (j.find("parent") != j.end()) {
		j.at("parent").get_to(m_parentNameFromJson);
	}
}