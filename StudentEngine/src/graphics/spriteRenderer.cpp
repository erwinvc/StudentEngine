#include "stdafx.h"


void SpriteRenderer::Draw(const RenderingPipeline* pipeline) {
	m_shader->Bind();
	m_shader->Set("_Projection", pipeline->m_camera->GetProjectionMatrix());

	Line(0, 0, GetApp()->GetPipeline()->m_camera->GetViewport().z, GetApp()->GetPipeline()->m_camera->GetViewport().w, Color::White(), 1);
	
	for (uint32 i = 0; i < m_textures.size(); i++) {
		m_textures[i]->Bind(i);
	}

	m_instancedRenderer->Draw();
	m_textures.clear();
}