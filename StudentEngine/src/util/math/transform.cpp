#include "stdafx.h"

bool Transform::CollidesWith(GameObject* other, float xOffset, float yOffset) {
	if (other == m_gameObject) return false;
	
	bool withinX = Math::Within(XMin() + xOffset, XMax() + xOffset, other->m_transform.XMin(), other->m_transform.XMax());
	bool withinY = Math::Within(YMin() + yOffset, YMax() + yOffset, other->m_transform.YMin(), other->m_transform.YMax());

	if (withinX && withinY) return true;
	return false;
}

void to_json(nlohmann::json& j, const Transform& obj) {
	j = nlohmann::json{{"position", obj.m_position}, {"size", obj.m_size}, {"rotation", obj.m_rotation}};
}

void from_json(const nlohmann::json& j, Transform& obj) {
	j.at("position").get_to(obj.m_position);
	j.at("size").get_to(obj.m_size);
	j.at("rotation").get_to(obj.m_rotation);
}