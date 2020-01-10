#include "stdafx.h"

void PlayState::Initialize() {
	m_playCamera = new Camera();
}

PlayState::~PlayState() {
	delete m_playCamera;
}

void PlayState::Update(const TimeStep& time) {
	if (m_restarting) {
		m_scene->Clear();
		m_scene->GetHierarchy().Initialize();
		m_scene->Copy(States::EDIT->GetScene());
		m_restarting = false;
		m_playerScore = 0;
	}
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
	m_playerScore = 0;

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

	OnHUD();
}

void PlayState::OnHUD() {
	//Could be moved into its own class

	String scoreText = Format("%s%i", "Score: ", m_playerScore);
	ImGui::GetOverlayDrawList()->AddText(
		ImGui::GetIO().Fonts->Fonts[2],		//Grabbing the special HUD font that's larger for better quality. Changing the Font order around will mess this up.
		ImGui::GetFontSize()*2.0f, 
		AnchorPoints::GetAnchor(Anchors::TOPLEFT, ImVec2(50, 75)), 
		IM_COL32(255, 255, 255, 255), 
		scoreText.c_str());
}

void PlayState::AdjustScore(int value) {
	m_playerScore += value;
}

void PlayState::Restart() {
	m_restarting = true;
}