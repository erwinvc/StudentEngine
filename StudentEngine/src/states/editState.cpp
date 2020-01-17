#include "stdafx.h"

GameObject* m_sample;
void EditState::Initialize() {
	g_buttonGizmo = GetAssetManager()->Get<StreamedTexture>("ButtonGizmo");
	g_arrowGizmo = GetAssetManager()->Get<StreamedTexture>("ArrowGizmo");
	g_squareGizmo = GetAssetManager()->Get<StreamedTexture>("SquareGizmo");
	g_logo = GetAssetManager()->Get<StreamedTexture>("Logo");

	ResetScene(false);

	m_window->Initialize();
}

void EditState::ResetScene(bool fullReset) {
	Undo::Clear();
	m_scene->GetHierarchy().Clear();
	m_scene->GetHierarchy().Initialize();

	if (!fullReset) {
		ObjectFactory::CreateObject(EditorObjectType::TERRAIN, "Terrain")
			->SetSize(Vector2(2048.0f, 128.0f))
			->SetPosition(Vector2(960.0f, 224.0f));

		ObjectFactory::CreateObject(EditorObjectType::TERRAIN, "Flying Island")
			->SetSize(Vector2(512.0f, 64.0f))
			->SetPosition(Vector2(1045.0f, 400.0f));

		ObjectFactory::CreateObject(EditorObjectType::TERRAIN, "Flying Island")
			->SetSize(Vector2(512.0f, 64.0f))
			->SetPosition(Vector2(1445.0f, 600.0f));

		ObjectFactory::CreateObject(EditorObjectType::PICKUP_COIN, "Pickup Blue")
			->SetPosition(Vector2(1445.0f, 664.0f));

		ObjectFactory::CreateObject(EditorObjectType::PICKUP_COIN, "Pickup Blue")
			->SetPosition(Vector2(1400.0f, 664.0f));
		
		ObjectFactory::CreateObject(EditorObjectType::PICKUP_COIN, "Pickup Blue")
			->SetPosition(Vector2(704.0f, 320.0f));

		ObjectFactory::CreateObject(EditorObjectType::PICKUP_POWERUP, "Pickup Green")
			->SetPosition(Vector2(1600.0f, 320.0f));

		ObjectFactory::CreateObject(EditorObjectType::ENEMY_STATIC, "Static Enemy")
			->SetPosition(Vector2(1376.0f, 320.0f));

		ObjectFactory::CreateObject(EditorObjectType::ENEMY_WALKING, "Walking Enemy")
			->SetPosition(Vector2(1233.0f, 475.0f));
	}

	ObjectFactory::CreateObject(EditorObjectType::PLAYER, "Player")
		->SetPosition(Vector2(192.0f, 320.0f));

	ObjectFactory::CreateObject(EditorObjectType::GOAL, "Goal")
		->SetPosition(Vector2(1536.0f, 320.0f));
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

	GameObject* selected = m_scene->m_hierarchy.GetSelected();
	if (selected) {
		if (!GetEditorWindow()->IsVIPObject(selected) && KeyJustDown('X') && KeyDown(LCTRL)) {
			GameObject* obj = ObjectFactory::CreateObject(selected->GetObjectType(), selected->m_name);
			int id = obj->m_id;
			*obj = *selected;
			obj->m_id = id;
			obj->m_name += "+";
		}
		if (KeyJustDown(VK_DELETE) && !GetEditorWindow()->IsVIPObject(selected)) {
			Undo::Record(selected);
			Undo::FinishRecording(true);
			selected->Destroy();
			m_scene->m_hierarchy.SetSelected(nullptr);
		}
	}

	if (m_scene->m_hierarchy.UpdateSelected(time, m_scene->GetCursorWorldPosition())) return;

	if (ButtonDoubleClicked(VK_MOUSE_LEFT)) {
		GameObject* dcObj = m_scene->GetGameObjectUnderMouse();
		if (dcObj) GetEditorWindow()->OpenInspector();

	}

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