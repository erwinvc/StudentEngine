#include "stdafx.h"
Texture* EditorManager::g_logo;
Texture* EditorManager::g_buttonGizmo;
Texture* EditorManager::g_arrowGizmo;
Texture* EditorManager::g_squareGizmo;

//Color colors[1000000];
void EditorManager::Initialize() {
	//for (int i = 0; i < 1000000; i++) {
	//	colors[i] = Color::RandomPrimary();
	//}
	g_buttonGizmo = GetAssetManager()->Get<Texture>("ButtonGizmo");
	g_arrowGizmo = GetAssetManager()->Get<Texture>("ArrowGizmo");
	g_squareGizmo = GetAssetManager()->Get<Texture>("SquareGizmo");
	g_logo = GetAssetManager()->Get<Texture>("Logo");

	//m_sample = &AddGameObject(new GameObject("Object 1"))
	//	.SetSize(Vector2(500, 200))
	//	.SetPosition(Vector2(GetApp()->GetPipeline()->m_camera->GetViewport().z / 2, GetApp()->GetPipeline()->m_camera->GetViewport().w / 2));

	AddGameObject(new GameObject("Object 2"))
		.SetSize(Vector2(500, 500))
		.SetPosition(Vector2(300.0f, GetApp()->GetPipeline()->m_camera->GetViewport().w / 2))
		.SetTexture(g_logo);
}

static float offset = 0;
float a = 0;
static float m_selectedOutlineValue = 0;

void EditorManager::Update(const TimeStep& time) {
	m_selectedOutlineValue += time.GetSeconds();
	Vector3 ray = GroundRaycast::GetMousePosition(GetApp()->GetPipeline()->m_camera);
	m_mouseRayPosition = GroundRaycast::GetGroundPosition(GetApp()->GetPipeline()->m_camera, ray, 1.0f);
	//m_sample->m_transform.m_position = m_mouseRayPosition;
	//m_sample->SetRotation(Math::Sin(m_selectedOutlineValue));
	//a += 0.5f * time.GetSeconds();
	//m_sample->m_transform.m_position.x = Math::Sin(a) * 100 + 500;
	//m_sample->m_sprite.m_color = Color(0.1f, 0.1f, 0.1f, 1.0);

	UpdateSelected(time);
	offset += time.GetSeconds() * 5;

	if (KeyDown(LCTRL)) {
		if (KeyJustDown('Z')) Undo::UndoOne();
		if (KeyJustDown('Y')) Undo::RedoOne();
	}
}

void EditorManager::UpdateSelected(const TimeStep& time) {
	if (m_hierarchy.UpdateSelected(time, m_mouseRayPosition)) return;
	if (ButtonJustDown(VK_MOUSE_LEFT)) {
		for (int i = (int)m_hierarchy.m_gameObjects.size() - 1; i >= 0; i--) {
			GameObject*& gObj = m_hierarchy.m_gameObjects[i];
			if (gObj->m_transform.IsPointWithin(m_mouseRayPosition.x, m_mouseRayPosition.y)) {
				m_hierarchy.m_selected = gObj;
				return;
			}
		}
		m_hierarchy.m_selected = nullptr;
	}
}


void EditorManager::Draw(RenderingPipeline* pipeline) {
	m_hierarchy.Draw(pipeline);

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