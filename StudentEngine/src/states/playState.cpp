#include "stdafx.h"

void PlayState::Initialize() {

}

void PlayState::Update(const TimeStep& time) {
	GetEditorManager()->GetHierarchy().Update(time);
}

void PlayState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetCamera()->SetViewport(GetEditorWindow()->GetViewport());
	GetEditorManager()->GetHierarchy().Draw(pipeline);
}

void PlayState::PostDraw(RenderingPipeline* pipeline) {

}

void PlayState::PostImGuiDraw(RenderingPipeline* pipeline) {

}

void PlayState::EnterState() {
	editorCamera = *GetCamera();
	Camera& cameraObject = *GetApp()->GetPipeline()->GetCamera();
	cameraObject = Camera();

	Hierarchy hierarchy = GetEditorManager()->GetHierarchy();
	hierarchy.m_selected = nullptr;
	for (int i = 0; i < hierarchy.m_gameObjects.size(); i++) {
		editorGameObjects.push_back(*hierarchy.m_gameObjects[i]);
	}
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void PlayState::ExitState() {
	Camera& cameraObject = *GetApp()->GetPipeline()->GetCamera();
	cameraObject = editorCamera;

	Hierarchy hierarchy = GetEditorManager()->GetHierarchy();
	for (int i = 0; i < editorGameObjects.size(); i++) {
		*hierarchy.m_gameObjects[i] = editorGameObjects[i];
	}
	editorGameObjects.clear();
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void PlayState::OnImGui() {
	GetEditorWindow()->OnImGui();
}