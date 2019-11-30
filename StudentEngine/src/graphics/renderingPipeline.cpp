#include "stdafx.h"

void RenderingPipeline::Initialize() {
	m_finalFBO = GetFrameBufferManager()->Create("Final", FBOScale::FULL, false);
	m_finalTexture = m_finalFBO->AddBuffer("Final", TextureParameters(RGB, RGB, NEAREST, CLAMP_TO_BORDER), FBOAttachment::COLOR);
	m_finalFBO->SetClearColor(Color::Black());
	m_camera = new OrthographicCamera();
	m_camera->SetViewport(0, 0, 1920, 1080);

	Texture* defaultTexture = GetAssetManager()->Get<Texture>("White");
	
	m_spriteRenderer = new SpriteRenderer(defaultTexture);
}

void RenderingPipeline::Update(const TimeStep time) {
	m_camera->Update(time);
}
void RenderingPipeline::Begin() {
	m_spriteRenderer->Begin();
	if (m_wireframe) {
		GL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
}

void RenderingPipeline::End() {
	m_finalFBO->Bind();
	m_finalFBO->Clear();
	m_spriteRenderer->End();
	GLUtils::EnableBlending();
	m_spriteRenderer->Draw(this);
	GLUtils::DisableBlending();
	if (m_wireframe) GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	GetFrameBufferManager()->BindDefaultFBO();
}

void RenderingPipeline::OnImGui() {
	if (ImGui::BeginTabItem("Pipeline")) {
		if (ImGui::Checkbox("Wireframe", &m_wireframe)) GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		ImGui::EndTabItem();
	}
}

RenderingPipeline::~RenderingPipeline() {
	delete m_camera;
	delete m_spriteRenderer;
}
