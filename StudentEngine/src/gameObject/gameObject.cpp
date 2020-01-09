#include "stdafx.h"

map<const char*, vector<const char*>> GameObject::m_validTextures = {};

void to_json(nlohmann::json& jsonObject, const GameObject& gameObject) {
	jsonObject = nlohmann::json{
		{ "name", gameObject.m_name },
		{ "type", gameObject.GetObjectType() },
		{ "texture", gameObject.m_sprite.m_texture->GetTexture()->GetName() },
		{ "transform", gameObject.m_transform }
	};

	if (gameObject.m_parent) {
		jsonObject["parent"] = gameObject.m_parent->m_name;
	}
}

void from_json(const nlohmann::json& jsonObject, GameObject& gameObject) {
	string objectName = jsonObject.at("name").get<string>();
	int objectType = jsonObject.at("type").get<int>();
	string objectTexture = jsonObject.at("texture").get<string>();

	GameObject* newObject = ObjectFactory::CreateObject((EditorObjectType)objectType, objectName);

	newObject->SetTexture(GetAssetManager()->Get<StreamedTexture>(objectTexture));

	Transform newTransform = jsonObject.at("transform").get<Transform>();
	newObject->SetPosition(Vector2(newTransform.m_position.x, newTransform.m_position.y));
	newObject->SetSize(Vector2(newTransform.m_size.x, newTransform.m_size.y));

	if (jsonObject.find("parent") != jsonObject.end()) {
		string objectParent = jsonObject.at("parent").get<string>();
		newObject->SetParent(GetEditorScene()->FindObjectByName(objectParent));
	}
}