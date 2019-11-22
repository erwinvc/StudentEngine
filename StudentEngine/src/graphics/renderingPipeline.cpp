#include "stdafx.h"

void RenderingPipeline::Initialize() {
	m_finalFBO = GetFrameBufferManager()->Create("Final", FBOScale::FULL, true);
	m_finalTexture = m_finalFBO->AddBuffer("Final", TextureParameters(), FBOAttachment::COLOR);
	m_finalFBO->SetClearColor(Color::White());
	m_camera = new OrthographicCamera();
	m_camera->SetViewport(0, 0, 1920, 1080);

	m_spriteRenderer = new SpriteRenderer();
}

void RenderingPipeline::Update(const TimeStep time) {
	m_camera->Update(time);
}
void RenderingPipeline::Draw() {
	m_finalFBO->Bind();
	m_finalFBO->Clear();
	GetFrameBufferManager()->BindDefaultFBO();
}
RenderingPipeline::~RenderingPipeline() {
	delete m_camera;
	delete m_spriteRenderer;
}
