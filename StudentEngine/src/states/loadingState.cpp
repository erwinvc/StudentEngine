#include "stdafx.h"

void LoadingState::Initialize() {

	m_loadingTexture[0] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading1", "res/Logo1.png"));
	m_loadingTexture[1] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading2", "res/Logo2.png"));
	m_loadingTexture[2] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading3", "res/Logo3.png"));
	m_loadingTexture[3] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading4", "res/Logo4.png"));
	
	GetAssetWatcher()->Initialize();
	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Keyboard", [] {GetKeyboard()->Initialize(GetApp()->GetWindow()); }));
	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Mouse", [] {GetMouse()->Initialize(GetApp()->GetWindow()); }));
	
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("ButtonGizmo", "res/buttonGizmo.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("ArrowGizmo", "res/arrowGizmo.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("SquareGizmo", "res/squareGizmo.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("Test Texture", "res/test.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("Logo", "res/testlogo.png", TextureParameters(RGBA, RGBA, NEAREST)));

	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Asset Watcher", [] {GetAssetWatcher()->Initialize(); }));
	//GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Rendering Pipeline", [] {GetApp()->GetPipeline()->Initialize(); }));
	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Undo", [] {Undo::Initialize(); }));
}

void LoadingState::Update(const TimeStep& time) {
	GetApp()->GetPipeline()->m_camera->Update(time);
	if (GetAssetManager()->GetActiveJobs() == 0) {
		GetStateManager()->SetState(States::EDIT);
	}
}

void LoadingState::Draw(RenderingPipeline* pipeline) {
	pipeline->m_camera->SetViewport(0, 0, GetApp()->GetWidth<float>(), GetApp()->GetHeight<float>());
	pipeline->Rect(0, 0, pipeline->Width(), pipeline->Height());
	pipeline->Rect(pipeline->Width() / 2, pipeline->Height() / 2, 200, 200, 0, Color::White(), m_loadingTexture[0]);
}

void LoadingState::EnterState() {
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void LoadingState::ExitState() {
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void LoadingState::OnImGui() {

}