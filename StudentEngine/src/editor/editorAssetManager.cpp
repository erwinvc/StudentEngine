#include "stdafx.h"

StreamedTexture* EditorAssetManager::g_folderTexture;

StreamedTexture* EditorAsset::GetTexture() {
	switch (m_type) {
		case EditorAssetType::SPRITE:
			return ((StreamedTexture*)m_asset);
	}
	return EditorAssetManager::g_folderTexture;
}

void EditorAssetManager::OnImGui() {
	if (ImGui::Begin("Assets", nullptr)) {

		ImGuiStyle& style = ImGui::GetStyle();
		float windowX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		bool goBack = false;

		if (m_currentFolder->m_parent != nullptr) {
			if (ImGui::NamedButton(g_folderTexture->GetTexture(), ImVec2(80, 60), ImVec2(0, 1), ImVec2(1, 0), 2, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1), "...", true)) {
				goBack = true;
			}
			ImGui::Tooltip("Go back");
			ImGui::SameLine();
		}

		for (int n = 0; n < m_currentFolder->m_assets.size(); n++) {
			EditorAsset* asset = m_currentFolder->m_assets[n];
			ImGui::PushID(n);

			if (ImGui::NamedButton(asset->GetTexture()->GetTexture(), ImVec2(80, 60), ImVec2(0, 1), ImVec2(1, 0), 2, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1), asset->m_name.c_str(), true)) {
				switch (asset->m_type) {
					case EditorAssetType::FOLDER:
						m_currentFolder = (EditorAssetFolder*)asset;
						break;
					case EditorAssetType::SPRITE:

						break;
					case EditorAssetType::ANIMATION:
						break;
				}
			}

			switch (asset->m_type) {
				case EditorAssetType::SPRITE: {
					static bool dragging = false;
					static bool recording = false;
					static StreamedTexture* tex;
					if (ImGui::IsItemActive()) {
						dragging = true;
						GameObject* obj = GetEditorManager()->GetGameObjectUnderMouse();
						if (obj) obj->m_sprite.m_singleFrameTexture = asset->GetTexture();
						tex = asset->GetTexture();
						ImGuiIO io = ImGui::GetIO();
						ImDrawList* draw_list = ImGui::GetWindowDrawList();
						draw_list->PushClipRectFullScreen();
						Vector2 aspect(0.75f, 1.0f);
						aspect *= asset->GetTexture()->GetTexture()->GetAspect();
						ImVec2 size(aspect.x * 80, aspect.y * 60);
						draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
						draw_list->AddImage((void*)asset->GetTexture()->GetTexture()->GetHandle(), io.MousePos - size / 2, io.MousePos + size / 2, ImVec2(0, 1), ImVec2(1, 0));
						draw_list->PopClipRect();
					} else if (dragging && ImGui::IsMouseReleased(0)) {
						GameObject* obj = GetEditorManager()->GetGameObjectUnderMouse();
						dragging = false;
						if (obj) {
							Undo::Record(obj);
							obj->m_sprite.m_texture = tex;
							Undo::FinishRecording();
						}
					}
				}break;
			}
			ImGui::Tooltip(asset->m_name.c_str());

			float lastButtonX = ImGui::GetItemRectMax().x;
			float nextButtonX = lastButtonX + style.ItemSpacing.x + 80;
			if (n + 1 < m_assets->m_assets.size() && nextButtonX < windowX)
				ImGui::SameLine();
			ImGui::PopID();
		}
		if (goBack) m_currentFolder = (EditorAssetFolder*)m_currentFolder->m_parent;
	}
	ImGui::End();
}