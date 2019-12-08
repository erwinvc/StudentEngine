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
	editorZoom = GetCamera()->GetZoom();
	GetCamera()->SetZoom(1.0f);
	GetCamera()->UpdateZoom(TimeStep());
	Hierarchy& hierarchy = GetEditorManager()->GetHierarchy();
	hierarchy.m_selected = nullptr;
	for (int i = 0; i < hierarchy.m_gameObjects.size(); i++) {
		m_hierarchy->AddGameObject(hierarchy.m_gameObjects[i]->Copy());
	}

	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void PlayState::ExitState() {
	GetCamera()->SetZoom(editorZoom);
	GetCamera()->UpdateZoom(TimeStep());

	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void PlayState::OnImGui() {
	GetEditorWindow()->OnImGui();
}