#include "stdafx.h"

PlayState::PlayState() {
	editorGameObjects = vector<GameObject>();
}

void PlayState::Initialize()
{
	
}

void PlayState::Update(const TimeStep& time) {

}
void PlayState::Draw(RenderingPipeline* pipeline) {

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

}