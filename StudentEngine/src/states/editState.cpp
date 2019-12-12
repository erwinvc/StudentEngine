#include "stdafx.h"

void EditState::Initialize() {
	g_buttonGizmo = GetAssetManager()->Get<StreamedTexture>("ButtonGizmo");
	g_arrowGizmo = GetAssetManager()->Get<StreamedTexture>("ArrowGizmo");
	g_squareGizmo = GetAssetManager()->Get<StreamedTexture>("SquareGizmo");
	g_logo = GetAssetManager()->Get<StreamedTexture>("Logo");

	AddGameObject(new GameObject("Object 1"))
		.SetSize(Vector2(500, 500))
		.SetPosition(Vector2(300.0f, GetCamera()->GetRelativeViewport().w / 2))
		.SetTexture(g_logo);

	StreamedTexture* playerSprite = GetAssetManager()->Get<StreamedTexture>("PlayerCat");
	AddGameObject(new PlayerObject("Player Object", 5))
		.SetSize(Vector2(64, 64))
		.SetPosition(Vector2(500.0f, 500.0f))
		.SetTexture(playerSprite);

	m_window->Initialize();
}

EditState::EditState() : BaseState("Edit") {
	m_assetManager = new EditorAssetManager();
	m_window = new EditorWindow();
}
EditState::~EditState() {
	delete m_assetManager;
	delete m_window;
	m_hierarchy.Clear();
}

GameObject* EditState::GetGameObjectUnderMouse() {
	if (!GetEditorWindow()->IsMouseInViewport()) {
		return nullptr;
	}
	for (int i = (int)m_hierarchy.m_gameObjects.size() - 1; i >= 0; i--) {
		GameObject*& gObj = m_hierarchy.m_gameObjects[i];
		if (gObj->m_transform.IsPointWithin(m_mouseRayPosition.x, m_mouseRayPosition.y)) {
			return gObj;
		}
	}
	return nullptr;
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

	if (m_hierarchy.UpdateSelected(time, m_mouseRayPosition)) return;

	if (ButtonJustDown(VK_MOUSE_LEFT)) {
		// Play bloop sound for testing purposes.
		//GetAudioManager()->PlayAudio(GetAssetManager()->Get<Audio>("BloopSound"));

		GameObject* obj = GetGameObjectUnderMouse();
		m_hierarchy.SetSelected(obj);
		GetInspector()->SetSelected(obj);
	}
}

void EditState::Update(const TimeStep& time) {
	m_window->Update(time);

	EditorGrid::Update(time);
	Vector3 ray = GroundRaycast::GetMousePosition(GetCamera());
	m_mouseRayPosition = GroundRaycast::GetGroundPosition(GetCamera(), ray, 1.0f);

	//m_sample->m_transform.m_position = m_mouseRayPosition;
	//m_sample->SetRotation(Math::Sin(m_selectedOutlineValue));
	//a += 0.5f * time.GetSeconds();
	//m_sample->m_transform.m_position.x = Math::Sin(a) * 100 + 500;
	//m_sample->m_sprite.m_color = Color(0.1f, 0.1f, 0.1f, 1.0);

	if (KeyDown(LCTRL)) {
		if (KeyJustDown('Z')) Undo::UndoOne();
		if (KeyJustDown('Y')) Undo::RedoOne();
	}

	EditorControls(time);
}

void EditState::Draw(RenderingPipeline* pipeline) {
	GetFrameBufferManager()->OnResize(GetEditorWindow()->GetViewport().z, GetEditorWindow()->GetViewport().w);
	GetCamera()->SetViewport(GetEditorWindow()->GetViewport());

	m_hierarchy.Draw(pipeline);
	m_hierarchy.EditorDraw(pipeline);
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