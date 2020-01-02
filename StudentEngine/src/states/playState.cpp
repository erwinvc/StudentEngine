#include "stdafx.h"

void PlayState::Initialize() {
	m_playCamera = new Camera();
}

PlayState::~PlayState() {
	delete m_playCamera;
}

void PlayState::Update(const TimeStep& time) {
	m_scene->Update(time);
}

void PlayState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetCamera()->SetViewport(GetEditorWindow()->GetViewport());
	m_scene->Draw(pipeline);
}

void PlayState::PostDraw(RenderingPipeline* pipeline) {

}

void PlayState::PostImGuiDraw(RenderingPipeline* pipeline) {

}

void PlayState::EnterState() {
	m_scene->Clear();
	m_scene->GetHierarchy().Initialize();
	m_scene->Copy(States::EDIT->GetScene());

	m_editorCamera = GetCamera();
	GetApp()->GetPipeline()->SetCamera(m_playCamera);

	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}



void PlayState::ExitState() {
	Camera& cameraObject = *GetApp()->GetPipeline()->GetCamera();
	GetApp()->GetPipeline()->SetCamera(m_editorCamera);

	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void PlayState::OnImGui() {
	GetEditorWindow()->OnImGui();
}