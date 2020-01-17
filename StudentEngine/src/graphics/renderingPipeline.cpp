#include "stdafx.h"
#include "util/meshGenerator.h"

void RenderingPipeline::Initialize() {
	m_finalFBO = GetFrameBufferManager()->Create("Final", FBOScale::FULL, false);
	m_finalTexture = m_finalFBO->AddBuffer("Final", TextureParameters(RGB, RGB, NEAREST, CLAMP_TO_BORDER), FBOAttachment::COLOR);
	m_finalFBO->SetClearColor(Color::Black());
	m_mainCamera = new Camera();
	m_mainCamera->SetViewport(0, 0, 1920, 1080);
	m_activeCamera = m_mainCamera;
	m_quad = MeshGenerator::Quad();
	m_spriteRenderer = new SpriteRenderer(GetAssetManager()->GetNullTexture());
	m_particleSystem = new ParticleSystem();
	m_initialized = true;
}

void RenderingPipeline::Update(const TimeStep time) {
	m_activeCamera->Update(time);
	m_particleSystem->Update(time);
}
void RenderingPipeline::Begin() {
	GLUtils::EnableBlending();
	m_spriteRenderer->Begin();
	if (m_wireframe) {
		GL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
}

void RenderingPipeline::EndSpriteRenderer() {
	m_particleSystem->Draw(this);
	m_finalFBO->Bind();
	m_finalFBO->Clear();
	m_spriteRenderer->End();
	m_spriteRenderer->Draw(this);
}

void RenderingPipeline::Finish() {
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
	delete m_mainCamera;
	delete m_spriteRenderer;
	delete m_particleSystem;
	delete m_quad;
}
