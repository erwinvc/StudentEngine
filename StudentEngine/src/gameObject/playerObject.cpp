#include "stdafx.h"

bool m_isJumping = false;

PlayerObject::PlayerObject(const String& name) : GameObject(name, true) {
	SetSize(Vector2(64, 64));
	SetTexture(GetAssetManager()->Get<StreamedTexture>("GreyCat"));
	SetMovementSpeed(0.5);
	m_layer = "Foreground";
	m_invincible = false;
}

EditorObjectType PlayerObject::GetObjectType() const {
	return EditorObjectType::PLAYER;
}

void PlayerObject::Update(const TimeStep& time) {
	Audio* backgroundMusic = GetAssetManager()->Get<Audio>("BackgroundMusic");
	backgroundMusic->SetLooping(true);
	if (!GetAudioManager()->IsPlaying(this, backgroundMusic)) {
		GetAudioManager()->Play(this, backgroundMusic);
	}

	// Movement
	m_physicsObject.m_velocity.y -= m_physicsObject.m_gravity * time;
	float decay = 1 / (1 + (time * m_physicsObject.m_friction));
	m_physicsObject.m_velocity.x *= decay;

	Audio* walkSound = GetAssetManager()->Get<Audio>("GrassWalkSound");
	Audio* jumpSound = GetAssetManager()->Get<Audio>("CatMeowSound");

	if (GetKeyboard()->KeyDown('A')) {
		if (!GetAudioManager()->IsPlaying(this, walkSound)) {
			GetAudioManager()->Play(this, walkSound);
		}
		m_physicsObject.m_velocity.x -= m_movementSpeed * time;
	}

	if (GetKeyboard()->KeyDown('D')) {
		if (!GetAudioManager()->IsPlaying(this, walkSound)) {
			GetAudioManager()->Play(this, walkSound);
		}
		m_physicsObject.m_velocity.x += m_movementSpeed * time;
	}

	if ((!GetKeyboard()->KeyDown('D') && !GetKeyboard()->KeyDown('A')) || !m_physicsObject.m_isGrounded) {
		GetAudioManager()->Stop(this, walkSound);
	}

	if (m_physicsObject.m_isGrounded && KeyJustDown(VK_SPACE)) {
		GetAudioManager()->Play(this, jumpSound);
		m_physicsObject.m_velocity.y = 20;
		m_physicsObject.m_isGrounded = false;
		m_isJumping = true;
	}

	if (!KeyDown(VK_SPACE)) m_isJumping = false;

	if (!m_physicsObject.m_isGrounded && KeyDown(VK_SPACE) && m_isJumping) {
		if (m_physicsObject.m_velocity.y > 0) {
			m_physicsObject.m_velocity.y += (0.075f * m_physicsObject.m_velocity.y) + 0.05f;
		}
	}

	//Stops mid-air jump
	if (m_physicsObject.m_velocity.y < 0) {
		m_physicsObject.m_isGrounded = false;
	}

	// Invincibility
	if (m_invincible) {
		Audio* powerupSound = GetAssetManager()->Get<Audio>("PowerupActiveSound");
		if (!GetAudioManager()->IsPlaying(this, powerupSound)) {
			GetAudioManager()->Pause(this, backgroundMusic);
			GetAudioManager()->Play(this, powerupSound);
		}
		int colorTime = (int)Math::Floor(m_invincibilityTimer.Get(Timer::SECONDS) * 1000) / 10 % 2;
		SetColor(colorTime == 0 ? Color::Yellow() : Color::White());
		if ((m_invincibilityTimer.Get(Timer::SECONDS) * 10) >= 3) {
			GetAudioManager()->Stop(this, powerupSound);
			GetAudioManager()->Unpause(this, backgroundMusic);
			SetInvinciblity(false);
			SetColor(Color::White());
		}
	}

	if (m_transform.m_position.y < -2500) static_cast<PlayState*>(GetStateManager()->GetState())->Restart();

	m_physicsObject.Update(time);
}

GameObject* PlayerObject::Copy() {
	return new PlayerObject(*this);
}

void PlayerObject::InspectorDraw() {
	GameObject::InspectorDraw();
	InspectorDrawer::Float("Movement speed", m_movementSpeed);
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Player Texture");
	float width = ImGui::GetContentRegionAvail().x;
	const char* buttonText = "Select";
	ImGui::SameLine(width - 8.0 - ImGui::CalcTextSize(m_sprite.m_texture->GetName().c_str(), NULL, true).x - ImGui::CalcTextSize(buttonText, NULL, true).x);
	ImGui::LabelText("##playerTexture", m_sprite.m_texture->GetName().c_str());
	ImGui::SameLine(width - ImGui::CalcTextSize(buttonText, NULL, true).x);
	if (ImGui::Button("Select")) {
		GetAssetSelect()->PrepareValidTextures("Player", [&](AssetBase* asset) {
			GameObject* selectedObject = GetEditorScene()->GetHierarchy().GetSelected();
			if (selectedObject->IsOfType<PlayerObject>()) {
				static_cast<PlayerObject*>(selectedObject->SetTexture((StreamedTexture*)asset));
			}
			});
		GetAssetSelect()->Open();
	}
}

PlayerObject* PlayerObject::SetMovementSpeed(float speed) {
	m_movementSpeed = speed;
	return this;
}

PlayerObject* PlayerObject::SetInvinciblity(bool invincible) {
	m_invincible = invincible;
	if (invincible) {
		m_invincibilityTimer = Timer();
	}
	return this;
}

bool PlayerObject::GetInvinciblity() {
	return m_invincible;
}