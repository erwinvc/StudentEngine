#include "stdafx.h"


void SpriteRenderer::Draw() {
	m_shader->Bind();
	m_shader->Set("_Projection", GetPipeline()->m_camera->GetProjectionMatrix());


	for (uint32 i = 0; i < m_textures.size(); i++) {
		m_textures[i]->Bind(i);
	}

	m_instancedRenderer->Draw();
	m_textures.clear();
}