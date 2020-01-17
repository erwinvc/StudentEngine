#include "stdafx.h"

bool m_isJumping = false;

PlayerObject::PlayerObject(const String& name) : GameObject(name, true) {
	SetSize(Vector2(64, 64));
	SetAtlasValues(4, 4, 0.125f);

	m_idle = GetAssetManager()->Get<StreamedTexture>("PlayerOne-idle");
	m_runLeft = GetAssetManager()->Get<StreamedTexture>("PlayerOne-runLeft");
	m_runRight = GetAssetManager()->Get<StreamedTexture>("PlayerOne-runRight");
	m_jumpLeft = GetAssetManager()->Get<StreamedTexture>("PlayerOne-jumpLeft");
	m_jumpRight = GetAssetManager()->Get<StreamedTexture>("PlayerOne-jumpRight");
	m_fallLeft = GetAssetManager()->Get<StreamedTexture>("PlayerOne-fallLeft");
	m_fallRight = GetAssetManager()->Get<StreamedTexture>("PlayerOne-fallRight");

	SetTexture(m_idle);

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
	Audio* jumpSound = GetAssetManager()->Get<Audio>("JumpSound");

	if (GetKeyboard()->KeyDown('A')) {
		if (m_usingMultiSheet && m_physicsObject.m_isGrounded) {
			SetAtlasValues(6, 6, 0.125f);
			SetTexture(m_runLeft);
		}
		if (!GetAudioManager()->IsPlaying(this, walkSound)) {
			GetAudioManager()->Play(this, walkSound);
		}
		m_physicsObject.m_velocity.x -= m_movementSpeed * time;
	}

	if (GetKeyboard()->KeyDown('D')) {
		if (m_usingMultiSheet && m_physicsObject.m_isGrounded) {
			SetAtlasValues(6, 6, 0.125f);
			SetTexture(m_runRight);
		}
		if (!GetAudioManager()->IsPlaying(this, walkSound)) {
			GetAudioManager()->Play(this, walkSound);
		}
		m_physicsObject.m_velocity.x += m_movementSpeed * time;
	}

	if ((!GetKeyboard()->KeyDown('D') && !GetKeyboard()->KeyDown('A')) || !m_physicsObject.m_isGrounded) {
		GetAudioManager()->Stop(this, walkSound);
		if (m_usingMultiSheet && m_physicsObject.m_isGrounded) {
			SetAtlasValues(4, 4, 0.125f);
			SetTexture(m_idle);
		}
	}

	if (m_physicsObject.m_isGrounded && KeyJustDown(VK_SPACE)) {
		GetAudioManager()->Play(this, jumpSound);
		m_physicsObject.m_velocity.y = 20;
		m_physicsObject.m_isGrounded = false;
		m_isJumping = true;
		if (m_usingMultiSheet) {
			SetAtlasValues(8, 8, 0.125f);
			if (m_physicsObject.m_velocity.x < 0)
				SetTexture(m_jumpLeft);
			else
				SetTexture(m_jumpRight);
		}
	}

	if (!KeyDown(VK_SPACE)) m_isJumping = false;

	if (!m_physicsObject.m_isGrounded && m_physicsObject.m_velocity < 0) {
		if (m_usingMultiSheet) {
			SetAtlasValues(2, 2, 0.125f);
			if (m_physicsObject.m_velocity.x < 0)
				SetTexture(m_fallLeft);
			else
				SetTexture(m_fallRight);
		}
	}

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
		if ((int) ( 100 * GetApp()->GetTotalFrameTime()) % 2 == 0)
			GetParticleSystem()->Add(m_transform.m_position.x, m_transform.m_position.y, Math::RandomFloat(-5, 5), Math::RandomFloat(10, 15), 0.125f, Math::RandomFloat(10, 18), Math::RandomFloat(10, 18), 0.01f, Math::RandomFloat(300, 800), 0, Color::RandomPrimary());
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

	if (m_transform.m_position.y < -2500) static_cast<PlayState*>(GetStateManager()->GetState())->GameOver();

	m_physicsObject.Update(time);
}

GameObject* PlayerObject::Copy() {
	return new PlayerObject(*this);
}

void PlayerObject::InspectorDraw() {
	GameObject::InspectorDraw();
	InspectorDrawer::Float(this, "Movement speed", m_movementSpeed);
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
				m_usingMultiSheet = (asset->GetName() == String("PlayerOne-idle"));

				if (m_usingMultiSheet) {
					SetAtlasValues(4, 4, 0.125f);
				} else {
					// Necessary to reset the sprite
					m_sprite = Sprite();
					SetAtlasValues(1, 0, 0);
				}

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