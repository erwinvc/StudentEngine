#include "stdafx.h"

GameObject* m_sample;
void EditState::Initialize() {
	g_buttonGizmo = GetAssetManager()->Get<StreamedTexture>("ButtonGizmo");
	g_arrowGizmo = GetAssetManager()->Get<StreamedTexture>("ArrowGizmo");
	g_squareGizmo = GetAssetManager()->Get<StreamedTexture>("SquareGizmo");
	g_logo = GetAssetManager()->Get<StreamedTexture>("Logo");

	/*m_scene->AddGameObject(new GameObject("Object 1"))
		.SetSize(Vector2(500, 500))
		.SetPosition(Vector2(300.0f, GetCamera()->GetRelativeViewport().w / 2))
		.SetTexture(g_logo);*/

	StreamedTexture* playerSprite = GetAssetManager()->Get<StreamedTexture>("GreyCat");
	PlayerObject* po = new PlayerObject("Player Object", 0.5);
	po->SetSize(Vector2(64, 64))
		.SetPosition(Vector2(500.0f, 500.0f))
		.SetTexture(playerSprite);
	m_scene->AddGameObject(po);

	/*m_sample = &GetScene()->AddGameObject(new GameObject("A"))
		.SetSize(Vector2(500, 500))
		.SetPosition(m_scene->GetCursorWorldPosition())
		.SetTexture(GetAssetManager()->Get<StreamedTexture>("Logo"));*/

	for (int i = 0; i < 6; i++) {
		GetScene()->AddGameObject(new GameObject(Format("Animation test %d", i)))
			.SetSize(Vector2(50.0f + Math::RandomFloat(75.0f), 50.0f + Math::RandomFloat(75.0f)))
			.SetPosition(Vector2(250.0f + i * 125.0f, 250.0f))
			.SetAtlasValues(8, 8, 0.125f, +i * 8)
			.SetOnCollision([i](GameObject* ths, GameObject* other)
				{
					LOG("a");
					if (other->IsOfType<PlayerObject>()) ths->Destroy(); return false;
				})
			.SetTexture(GetAssetManager()->Get<StreamedTexture>("Gems"));
	}

	m_window->Initialize();
}

EditState::EditState() : BaseState("Edit") {
	//m_assetManager = new EditorAssetManager();
	m_window = new EditorWindow();
}
EditState::~EditState() {
	//delete m_assetManager;
	delete m_window;
}

void EditState::EditorControls(const TimeStep& time) {
	static bool dragging = false;
	if (!ButtonDown(VK_MOUSE_MIDDLE)) dragging = false;
	if (dragging) {
		Vector2 delta = GetMouse()->GetDelta();
		delta.y *= -1;
		GetCamera()->m_position += delta * GetCamera()->GetZoom();
	}

	if (!GetEditorWindow()->IsMouseInViewport() || dragging) return;
	if (ButtonJustDown(VK_MOUSE_MIDDLE)) dragging = true;

	if (m_scene->m_hierarchy.UpdateSelected(time, m_scene->GetCursorWorldPosition())) return;

	if (ButtonJustDown(VK_MOUSE_LEFT)) {
		GameObject* obj = m_scene->GetGameObjectUnderMouse();
		m_scene->m_hierarchy.SetSelected(obj);

		GetInspector()->SetSelected(obj);
	}
}
void EditState::Update(const TimeStep& time) {
	m_window->Update(time);
	m_scene->Update(time);
	EditorGrid::Update(time);

	if (KeyDown(LCTRL)) {
		if (KeyJustDown('Z')) Undo::UndoOne();
		if (KeyJustDown('Y')) Undo::RedoOne();
	}

	// Testing Asset Selection Window
	if (KeyJustDown(GLFW_KEY_K)) {
		GetAssetSelect()->PrepareValidTextures("Player", [&](AssetBase* asset) {
			GameObject* playerObject = m_scene->FindObjectByName("Player Object");
			playerObject->SetTexture((StreamedTexture*)asset);
		});
		GetAssetSelect()->Open();
	}

	//if (KeyJustDown(VK_SPACE)) {
		//m_scene->m_quadtree.Insert(&GetScene()->AddGameObject(new GameObject("A"))
		//	.SetSize(Vector2(500, 500))
		//	.SetPosition(m_scene->GetCursorWorldPosition())
		//	.SetTexture(GetAssetManager()->Get<StreamedTexture>("Logo")).m_transform);
	//}

	EditorControls(time);
}

void EditState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetCamera()->SetViewport(GetEditorWindow()->GetViewport());

	m_scene->Draw(pipeline);

	EditorGrid::Draw(pipeline);
	//int count = 0;
	//float scale = 2.5f;
	//Vector4 vp = GetApp()->GetPipeline()->m_camera->GetViewport();
	//for (int y = 0; y < vp.w / scale; y++) {
	//	for (int x = 0; x < vp.z / scale; x++) {
	//		pipeline->Rect(x * scale, y * scale, 2, 2, colors[(int)((y * vp.w) + x + offset)], m_whiteTexture);
	//		count++;
	//	}
	//}
	//static int TIMER = 0;
	//Utils::DoTimedFunction(&TIMER, 250, [count] {LOG("%d", count); });
}

void EditState::PostDraw(RenderingPipeline* pipeline) {
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

EditState* GetEditor() { return (EditState*)States::EDIT; }