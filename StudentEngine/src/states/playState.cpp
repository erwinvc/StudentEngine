#include "stdafx.h"

Hierarchy* m_hierarchy;
void PlayState::Initialize() {
	m_hierarchy = new Hierarchy();
	m_playCamera = new Camera();
}

PlayState::~PlayState() {
	delete m_hierarchy;
	delete m_playCamera;
}

void PlayState::Update(const TimeStep& time) {
	m_hierarchy->Update(time);
}

void PlayState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetCamera()->SetViewport(GetEditorWindow()->GetViewport());
	m_hierarchy->Draw(pipeline);
}

void PlayState::PostDraw(RenderingPipeline* pipeline) {

}

void PlayState::PostImGuiDraw(RenderingPipeline* pipeline) {

}

void PlayState::EnterState() {
	m_hierarchy->Clear();
	m_editorCamera = GetCamera();
	GetApp()->GetPipeline()->SetCamera(m_playCamera);

	Hierarchy& hierarchy = GetScene()->GetHierarchy();
	hierarchy.SetSelected(nullptr);
	for (int i = 0; i < hierarchy.m_gameObjects.size(); i++) {
		m_hierarchy->AddGameObject(hierarchy.m_gameObjects[i]->Copy());
	}

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