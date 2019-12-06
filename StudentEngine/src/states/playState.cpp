#include "stdafx.h"

void PlayState::Initialize() {

}

void PlayState::Update(const TimeStep& time) {
	GetEditorManager()->GetHierarchy().Update(time);
}

void PlayState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetApp()->GetPipeline()->m_camera->SetViewport(GetEditorWindow()->GetViewport());
	GetEditorManager()->GetHierarchy().Draw(pipeline);
}

void PlayState::PostDraw(RenderingPipeline* pipeline) {

}

void PlayState::PostImGuiDraw(RenderingPipeline* pipeline) {

}

void PlayState::EnterState() {
	Hierarchy hierarchy = GetEditorManager()->GetHierarchy();
	for (int i = 0; i < hierarchy.m_gameObjects.size(); i++) {
		editorGameObjects.push_back(*hierarchy.m_gameObjects[i]);
		LOG("1: %f", editorGameObjects[i].m_transform.m_position.x);
	}
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void PlayState::ExitState() {
	Hierarchy hierarchy = GetEditorManager()->GetHierarchy();

	for (int i = 0; i < editorGameObjects.size(); i++) {
		*hierarchy.m_gameObjects[i] = editorGameObjects[i];
	}
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void PlayState::OnImGui() {
	GetEditorWindow()->OnImGui();
}