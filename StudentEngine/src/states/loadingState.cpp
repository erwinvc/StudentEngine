#include "stdafx.h"

void LoadingState::Initialize() {
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
	//if (GetAssetManager()->GetActiveJobs() == 0) {
		GetStateManager()->SetState(States::EDIT);
	//}
}

void LoadingState::Draw(RenderingPipeline* pipeline) {

}

void LoadingState::EnterState() {
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void LoadingState::ExitState() {
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void LoadingState::OnImGui() {

}