#include "stdafx.h"

Shader* m_basicShader;
int m_animationFrame = 0;
void LoadingState::Initialize() {
	GetShaderManager()->Create("Sprite", "res/shaders/sprite");
	m_basicShader = GetShaderManager()->Create("Basic", "res/shaders/basic");
	m_basicShader->Set("_Texture", 0);

	m_loadingTexture[0] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading1", "res/assets/textures/Logo1.png"));
	m_loadingTexture[1] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading2", "res/assets/textures/Logo2.png"));
	m_loadingTexture[2] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading3", "res/assets/textures/Logo3.png"));
	m_loadingTexture[3] = GetAssetManager()->ForceLoadAsset<StreamedTexture>(new TextureLoadJob("Loading4", "res/assets/textures/Logo4.png"));

	GetAssetManager()->ForceLoadAsset<int>(new CustomLoadJob("Thread Pool", [] {GetThreadPool()->Initialize(5); }));

	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Keyboard", [] {GetKeyboard()->Initialize(GetApp()->GetWindow()); }));
	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Mouse", [] {GetMouse()->Initialize(GetApp()->GetWindow()); }));
	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Audio", [] {GetAudioManager()->Initialize(); }));
	GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Asset Manager", [] {GetAssetManager()->Initialize(); }));
	//GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Undo", [] {Undo::Initialize(); }));
	//GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Editor Asset Manager", [] {GetEditorAssetManager()->Initialize(); }));
	//GetAssetManager()->AddToLoadQueue(new CustomLoadJob("ImGui Manager", [] {}, [] {  }));
	//GetAssetManager()->AddToLoadQueue(new CustomLoadJob("Editor Window", [] {  }));

	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("9slice", "res/assets/textures/tile.png", TextureParameters(RGBA, RGBA, NEAREST, REPEAT)));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("Gems", "res/assets/textures/gems.png", TextureParameters(RGBA, RGBA, NEAREST, REPEAT)));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("ButtonGizmo", "res/assets/textures/buttonGizmo.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("ButtonGizmo", "res/assets/textures/buttonGizmo.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("ArrowGizmo", "res/assets/textures/arrowGizmo.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("SquareGizmo", "res/assets/textures/squareGizmo.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("Test Texture", "res/assets/textures/test.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("Logo", "res/assets/textures/testlogo.png", TextureParameters(RGBA, RGBA, NEAREST)));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PlayerOne-idle", "res/assets/textures/player1-idle.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PlayerOne-runLeft", "res/assets/textures/player1-runLeft.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PlayerOne-runRight", "res/assets/textures/player1-runRight.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PlayerOne-jumpLeft", "res/assets/textures/player1-jumpLeft.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PlayerOne-jumpRight", "res/assets/textures/player1-jumpRight.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PlayerOne-fallLeft", "res/assets/textures/player1-fallLeft.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PlayerOne-fallRight", "res/assets/textures/player1-fallRight.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("GreyCat", "res/assets/textures/greycat.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("BrownCat", "res/assets/textures/browncat.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("RedCat", "res/assets/textures/redcat.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("PurpleCat", "res/assets/textures/purplecat.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("BluePickup", "res/assets/textures/bluePickup.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("GreenPickup", "res/assets/textures/greenPickup.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("Goal", "res/assets/textures/goal.png"));
	GetAssetManager()->AddToLoadQueue(new TextureLoadJob("Particles", "res/assets/textures/particles.png"));
	
	GameObject::SetValidTextures("Player", { "PlayerOne-idle", "GreyCat", "BrownCat" });

	GetAssetManager()->AddToLoadQueue(new AudioLoadJob("GrassWalkSound", "res/assets/audio/grass_walk.wav"));
	GetAssetManager()->AddToLoadQueue(new AudioLoadJob("JumpSound", "res/assets/audio/jump_sound.wav"));
	GetAssetManager()->AddToLoadQueue(new AudioLoadJob("CoinPickupSound", "res/assets/audio/coin_pickup.wav"));
	GetAssetManager()->AddToLoadQueue(new AudioLoadJob("PowerupPickupSound", "res/assets/audio/powerup_pickup.wav"));
	GetAssetManager()->AddToLoadQueue(new AudioLoadJob("PowerupActiveSound", "res/assets/audio/powerup_active.wav"));
	GetAssetManager()->AddToLoadQueue(new AudioLoadJob("BackgroundMusic", "res/assets/audio/background_music.mp3"));

	//for (auto& file : std::filesystem::recursive_directory_iterator("res/Assets")) {
	//	if (file.path().extension().string().compare(".png") == 0) {
	//		Path path(file.path().string());
	//		String fullPath = path.GetFullPath();
	//		GetAssetManager()->AddToLoadQueue(new TextureLoadJob(fullPath, path));
	//		AssetBase* asset = GetAssetManager()->Get<StreamedTexture>(fullPath);
	//		GetEditorAssetManager()->AddAsset(path, asset);
	//	}
	//}

	for (auto& state : GetStateManager()->GetStates()) {
		if (state != this) {
			GetAssetManager()->AddToLoadQueue(new StateLoadJob(state));
		}
	}
}

void LoadingState::Update(const TimeStep& time) {
	m_animationFrame = 3 - (int)(time.TotalTime() / 100) % 4;
	if (GetAssetManager()->GetActiveJobs() == 0 && !m_fade/* && KeyJustDown(VK_SPACE)*/) {
		GetApp()->GetPipeline()->GetParticleSystem()->m_texture = GetAssetManager()->Get<StreamedTexture>("Particles");
		Tween::To(m_transparency, 0.0f, 1500.0f)->SetEase(Ease::INEXPO)->SetOnComplete([] {GetStateManager()->SetState(States::EDIT); });
		m_fade = true;
	}
}

void LoadingState::Draw(RenderingPipeline* pipeline) {
	if (m_fade) States::EDIT->Draw(pipeline);
}

void LoadingState::PostDraw(RenderingPipeline* pipeline) {
	if (m_fade) States::EDIT->PostDraw(pipeline);
}

void LoadingState::PostImGuiDraw(RenderingPipeline* pipeline) {
	GetCamera()->SetViewport(0, 0, GetApp()->GetWidth<float>(), GetApp()->GetHeight<float>());
	GetFrameBufferManager()->OnResize(GetApp()->GetWidth<float>(), GetApp()->GetHeight<float>());

	pipeline->Begin();
	pipeline->Rect(pipeline->Width() / 2, pipeline->Height() / 2, pipeline->Width(), pipeline->Height(), 0, Color::Black());
	pipeline->Rect(pipeline->Width() - 100, 100, 100, 100, 0, Color::White(), m_loadingTexture[m_animationFrame]);
	pipeline->EndSpriteRenderer();
	pipeline->Finish();

	m_basicShader->Bind();
	m_basicShader->Set("_Transparency", m_transparency);
	pipeline->GetFinalTexture()->Bind(0);
	pipeline->GetQuad()->Draw();
}

void LoadingState::EnterState() {
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void LoadingState::ExitState() {
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}

void LoadingState::OnImGui() {
	if (m_fade) States::EDIT->OnImGui();
}