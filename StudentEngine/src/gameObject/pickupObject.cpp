#include "stdafx.h"

PickupObject::PickupObject(const String& name) : GameObject(name, true) {

}

map<string, PickupType> PickupObject::typeMap = {
	{ "BluePickup", PickupType::COIN }, { "GreenPickup", PickupType::POWERUP }
};

EditorObjectType PickupObject::GetObjectType() const {
	return EditorObjectType::PICKUP;
}

GameObject* PickupObject::Copy() {
	return new PickupObject(*this);
}

void PickupObject::InspectorDraw() {
	GameObject::InspectorDraw();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Pickup Type");
	float width = ImGui::GetContentRegionAvail().x;
	const char* buttonText = "Select";
	ImGui::SameLine(width - 8.0 - ImGui::CalcTextSize(m_sprite.m_texture->GetName().c_str(), NULL, true).x - ImGui::CalcTextSize(buttonText, NULL, true).x);
	ImGui::LabelText("##pickupType", m_sprite.m_texture->GetName().c_str());
	ImGui::SameLine(width - ImGui::CalcTextSize(buttonText, NULL, true).x);
	if (ImGui::Button("Select")) {
		GetAssetSelect()->PrepareValidTextures("Pickup", [&](AssetBase* asset) {
			GameObject* selectedObject = GetEditorScene()->GetHierarchy().GetSelected();
			if (selectedObject->IsOfType<PickupObject>()) {
				static_cast<PickupObject*>(selectedObject->SetTexture((StreamedTexture*)asset))
					->SetPickupType(GetTypeFromTexture(asset->GetName()));
			}
			});
		GetAssetSelect()->Open();
	}
}

PickupObject* PickupObject::SetPickupType(PickupType type) {
	m_type = type;
	return this;
}

PickupType PickupObject::GetTypeFromTexture(string& textureName) {
	return typeMap[textureName];
}

void PickupObject::OnPickup() {
	switch (m_type) {
	case PickupType::COIN:
		((PlayState*)GetStateManager()->GetState())->AdjustScore(5);
		break;
	case PickupType::POWERUP:
		PlayerObject* playerObject = static_cast<PlayerObject*>(GetActiveScene()->FindObjectByName("Player"));
		playerObject->SetInvinciblity(true);
		break;
	}
}