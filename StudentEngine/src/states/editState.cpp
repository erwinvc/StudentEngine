#include "stdafx.h"

void EditState::Initialize() {
	GetEditorManager()->Initialize();
}

void EditState::Update(const TimeStep& time) {
	GetEditorManager()->Update(time);
	GetEditorWindow()->Update(time);
}

void EditState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetApp()->GetPipeline()->m_camera->SetViewport(GetEditorWindow()->GetViewport());
	GetEditorManager()->Draw(pipeline);
}

void EditState::PostDraw(RenderingPipeline* pipeline) {

}

void EditState::EnterState() {
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void EditState::ExitState() {
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void EditState::OnImGui() {
	GetEditorWindow()->OnImGui();
}