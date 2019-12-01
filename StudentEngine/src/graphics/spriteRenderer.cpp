#include "stdafx.h"


void SpriteRenderer::Draw(RenderingPipeline* pipeline) {
	m_shader->Bind();
	m_shader->Set("_Projection", pipeline->m_camera->GetProjectionMatrix());

	for (uint32 i = 0; i < m_textures.size(); i++) {
		m_textures[i]->Bind(i);
	}

	m_instancedRenderer->Draw();
}