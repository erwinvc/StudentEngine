#include "stdafx.h"

void EditState::Initialize() {
	GetEditorManager()->Initialize();
	GetEditorWindow()->Initialize();
}

void EditState::Update(const TimeStep& time) {
	GetEditorManager()->Update(time);
	GetEditorWindow()->Update(time);
}

void EditState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetCamera()->SetViewport(GetEditorWindow()->GetViewport());
	GetEditorManager()->Draw(pipeline);
}

void EditState::PostDraw(RenderingPipeline* pipeline) {
	GetEditorManager()->PostDraw(pipeline);
}

void EditState::PostImGuiDraw(RenderingPipeline* pipeline) {
}


void EditState::EnterState() {
	GetCamera()->SetZoomEnabled(true);
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void EditState::ExitState() {
	GetCamera()->SetZoomEnabled(false);
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void EditState::OnImGui() {
	GetEditorWindow()->OnImGui();
}