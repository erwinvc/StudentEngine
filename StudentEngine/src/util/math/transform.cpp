#include "stdafx.h"

bool Transform::CollidesWith(GameObject* other, float xOffset, float yOffset) {
	if (other == m_gameObject) return false;
	
	bool withinX = Math::Within(XMin() + xOffset, XMax() + xOffset, other->m_transform.XMin(), other->m_transform.XMax());
	bool withinY = Math::Within(YMin() + yOffset, YMax() + yOffset, other->m_transform.YMin(), other->m_transform.YMax());

	if (withinX && withinY) return true;
	return false;
}

void to_json(nlohmann::json& jsonObject, const Transform& transform) {
	jsonObject = nlohmann::json{
		{ "position", { {"x", transform.m_position.x }, {"y", transform.m_position.y} } },
		{ "size", { {"x", transform.m_size.x }, {"y", transform.m_size.y} } }
	};
}

void from_json(const nlohmann::json& jsonObject, Transform& transform) {
	nlohmann::json position = jsonObject.at("position").get<nlohmann::json>();
	nlohmann::json size = jsonObject.at("size").get<nlohmann::json>();

	transform.m_position = Vector2(position.at("x").get<float>(), position.at("y").get<float>());
	transform.m_size = Vector2(size.at("x").get<float>(), size.at("y").get<float>());
}