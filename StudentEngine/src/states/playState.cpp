#include "stdafx.h"

Hierarchy* m_hierarchy;
void PlayState::Initialize() {
	m_hierarchy = new Hierarchy();
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
	editorCamera = *GetCamera();
	Camera& cameraObject = *GetCamera();
	cameraObject = Camera();

	Hierarchy& hierarchy = GetEditorManager()->GetHierarchy();
	hierarchy.SetSelected(nullptr);
	for (int i = 0; i < hierarchy.m_gameObjects.size(); i++) {
		m_hierarchy->AddGameObject(hierarchy.m_gameObjects[i]->Copy());
	}

	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void PlayState::ExitState() {
	Camera& cameraObject = *GetApp()->GetPipeline()->GetCamera();
	cameraObject = editorCamera;

	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void PlayState::OnImGui() {
	GetEditorWindow()->OnImGui();
}