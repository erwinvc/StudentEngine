#include "stdafx.h"

void EditorManager::Initialize() {
	do {
		m_whiteTexture = GetAssetManager()->Get<Texture>("White");
		GetGLFiberManager()->GoToMainFiber();
	} while (m_whiteTexture == nullptr);

	AddGameObject(new GameObject(Vector2(GetApp()->GetPipeline()->m_camera->GetViewport().z / 2, GetApp()->GetPipeline()->m_camera->GetViewport().w / 2), Vector2(500, 500), m_whiteTexture));
}

void EditorManager::Update(const TimeStep& time) {
	Vector3 ray = GroundRaycast::GetMousePosition(GetApp()->GetPipeline()->m_camera);
	m_mouseRayPosition = GroundRaycast::GetGroundPosition(GetApp()->GetPipeline()->m_camera, ray, 1.0f);
}

void EditorManager::Draw(const RenderingPipeline* pipeline) {
	m_hierarchy.Draw(pipeline);
}