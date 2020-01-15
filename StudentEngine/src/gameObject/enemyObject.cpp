#include "stdafx.h"

EnemyObject::EnemyObject(const String& name) : GameObject(name, true) {

}

map<string, EnemyType> EnemyObject::typeMap = {
	{ "RedCat", EnemyType::STATIC }, { "PurpleCat", EnemyType::WALKING }
};

EditorObjectType EnemyObject::GetObjectType() const {
	return EditorObjectType::ENEMY;
}

GameObject* EnemyObject::Copy() {
	return new EnemyObject(*this);
}

void EnemyObject::Update(const TimeStep& time) {
	m_physicsObject.m_velocity.y -= m_physicsObject.m_gravity * time;
	if (m_type == EnemyType::WALKING) {
		float decay = 1 / (1 + (time * m_physicsObject.m_friction));
		m_physicsObject.m_velocity.x *= decay;
		float xVelocity = m_walkingDirection * m_movementSpeed * time;
		float xOffset = m_transform.m_position.x - xVelocity;
		float yOffset = m_transform.m_position.y - m_sprite.m_texture->GetTexture()->GetHeight() - 1;
		vector<GameObject*> backgroundObjects = GetActiveScene()->GetHierarchy().FindLayerByName("Background")->m_objects;
		/*LOG("%d", backgroundObjects.size());*/
		for (GameObject* other : GetActiveScene()->GetHierarchy().FindLayerByName("Background")->m_objects) {
			if (!m_transform.CollidesWith(other, xOffset, yOffset)) {
				m_walkingDirection *= -1;
			}
		}
		m_physicsObject.m_velocity.x -= xVelocity;
	}
	m_physicsObject.Update(time);
}

void EnemyObject::InspectorDraw() {
	GameObject::InspectorDraw();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Enemy Type");
	float width = ImGui::GetContentRegionAvail().x;
	const char* buttonText = "Select";
	ImGui::SameLine(width - 8.0 - ImGui::CalcTextSize(m_sprite.m_texture->GetName().c_str(), NULL, true).x - ImGui::CalcTextSize(buttonText, NULL, true).x);
	ImGui::LabelText("##enemyType", m_sprite.m_texture->GetName().c_str());
	ImGui::SameLine(width - ImGui::CalcTextSize(buttonText, NULL, true).x);
	if (ImGui::Button("Select")) {
		GetAssetSelect()->PrepareValidTextures("Enemy", [&](AssetBase* asset) {
			GameObject* selectedObject = GetEditorScene()->GetHierarchy().GetSelected();
			if (selectedObject->IsOfType<EnemyObject>()) {
				static_cast<EnemyObject*>(selectedObject->SetTexture((StreamedTexture*)asset))
					->SetEnemyType(GetTypeFromTexture(asset->GetName()));
			}
			});
		GetAssetSelect()->Open();
	}
}

EnemyObject* EnemyObject::SetEnemyType(EnemyType type) {
	m_type = type;
	return this;
}

EnemyType EnemyObject::GetTypeFromTexture(string& textureName) {
	return typeMap[textureName];
}