#include "stdafx.h"

float tim = 0;
Texture* m_whiteTexture;
bool once = true;
void SpriteRenderer::Draw() {
	m_shader->Bind();
	m_shader->Set("_Projection", GetPipeline()->m_camera->GetProjectionMatrix());
	Texture* tex = GetAssetManager()->Get<Texture>("Logo");

	if (once) {
		m_whiteTexture = new Texture(1, 1, (uint8*)Color::White().ToColor8(), false, TextureParameters(RGBA, RGBA, NEAREST, REPEAT));
		once = false;
	}
	if (tex) {
		tim += 0.00125f;
		float s = Math::Sin(tim) * 200;
		float c = Math::Cos(tim) * 200;
		tim += 0.00075f;
		float s2 = Math::Sin(tim) * 200;
		float c2 = Math::Cos(tim) * 200;
		Rect(GetPipeline()->m_camera->GetViewport().z / 2, GetPipeline()->m_camera->GetViewport().w / 2, GetPipeline()->m_camera->GetViewport().z, GetPipeline()->m_camera->GetViewport().w, Color(0.1f, 0.1f, 0.1f, 1.0f), m_whiteTexture);
		Rect(GetPipeline()->m_camera->GetViewport().z / 2, GetPipeline()->m_camera->GetViewport().w / 2, 400.0f, 400.0f, Color::White(), tex);
		Rect(GetPipeline()->m_camera->GetViewport().z / 2 - 200 + s, GetPipeline()->m_camera->GetViewport().w / 2 + 50, 400.0f, 400.0f, Color::Green(), tex);
		Rect(GetPipeline()->m_camera->GetViewport().z / 2 + s2 + 25.0f, GetPipeline()->m_camera->GetViewport().w / 2 - 100 + c, 400.0f, 400.0f, Color::Red(), tex);
		Rect(GetPipeline()->m_camera->GetViewport().z / 2 - 100, GetPipeline()->m_camera->GetViewport().w / 2 + c2, 400.0f, 400.0f, Color(1.0f, 0.0f, 0.4f, 0.3f), tex);
	}

	for (uint32 i = 0; i < m_textures.size(); i++) {
		m_textures[i]->Bind(i);
	}

	m_instancedRenderer->Draw();
	m_textures.clear();
}