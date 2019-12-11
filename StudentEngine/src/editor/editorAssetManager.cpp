#include "stdafx.h"

StreamedTexture* EditorAssetManager::g_folderTexture;

StreamedTexture* EditorAsset::GetTexture() {
	switch (m_type) {
		case EditorAssetType::SPRITE:
			return ((StreamedTexture*)m_asset);
	}
	return EditorAssetManager::g_folderTexture;
}

EditorAssetManager::~EditorAssetManager() {
	m_assets->Clear();
	delete m_assets;
}

void EditorAssetManager::Initialize() {
	g_folderTexture = GetAssetManager()->Get<StreamedTexture>("FolderIcon");
}

void EditorAssetManager::AddAsset(const Path& path, AssetBase* asset) {
	String dir = path.GetDirectory();
	if (!dir._Starts_with("res/Assets")) LOG_ERROR("Invalid asset path");
	String relativePath = dir.substr(10, dir.size());

	auto split = Utils::Split(relativePath, "/");
	EditorAssetFolder* folder = m_assets;

	for (auto& str : split) {
		if (str.empty()) continue;
		bool found = false;
		for (int i = 0; i < folder->m_assets.size(); i++) {
			if (folder->m_assets[i]->m_type == EditorAssetType::FOLDER && folder->m_assets[i]->m_name == str) {
				folder = (EditorAssetFolder*)folder->m_assets[i];
				found = true;
			}
		}
		if (!found) {
			EditorAssetFolder* newFolder = new EditorAssetFolder(str, folder);
			folder->m_assets.push_back(newFolder);
			folder->Sort();
			folder = newFolder;
		}
	}
	folder->m_assets.push_back(new EditorAsset(path.GetFileName(), path.GetFullPath(), asset, EditorAssetType::SPRITE, folder));
	folder->Sort();
}

void EditorAssetManager::RemoveAsset(const Path& path) {
	String dir = path.GetDirectory();
	if (!dir._Starts_with("res/Assets")) LOG_ERROR("Invalid asset path");
	String relativePath = dir.substr(10, dir.size());

	auto split = Utils::Split(relativePath, "/");
	EditorAssetFolder* folder = m_assets;
	bool removed = false;
	for (auto& str : split) {
		if (str.empty()) continue;
		for (int i = 0; i < folder->m_assets.size(); i++) {
			if (folder->m_assets[i]->m_type == EditorAssetType::FOLDER && folder->m_assets[i]->m_name == str) {
				folder = (EditorAssetFolder*)folder->m_assets[i];
			}
		}
	}

	for (int i = 0; i < folder->m_assets.size(); i++) {
		if (folder->m_assets[i]->m_fullName == path.GetFullPath()) {
			delete folder->m_assets[i];
			Utils::RemoveFromVector(folder->m_assets, folder->m_assets[i]);
			removed = true;
			break;
		}
	}
	if (!removed) {
		LOG_WARN("[~yAssets~x] failed to remove ~1%s~x from editor asset manager", path.GetFullPath().c_str());
	} else {
		if (folder->m_assets.size() == 0) {
			EditorAssetFolder* parent = (EditorAssetFolder*)folder->m_parent;
			delete folder;
			Utils::RemoveFromVector(parent->m_assets, (EditorAsset*)folder);
		}
	}
}

void EditorAssetManager::OnImGui() {
	if (ImGui::Begin("Assets", nullptr)) {

		ImGuiStyle& style = ImGui::GetStyle();
		float windowX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		bool goBack = false;

		if (m_currentFolder->m_parent != nullptr) {
			if (ImGui::NamedButton(g_folderTexture->GetTexture(), ImVec2(80, 60), ImVec2(0, 1), ImVec2(1, 0), 2, false, "...", true)) {
				goBack = true;
			}
			ImGui::Tooltip("Go back");
			ImGui::SameLine();
		}

		for (int n = 0; n < m_currentFolder->m_assets.size(); n++) {
			EditorAsset* asset = m_currentFolder->m_assets[n];
			ImGui::PushID(n);

			static int selected = 0;
			if (ImGui::NamedButton(asset->GetTexture()->GetTexture(), ImVec2(80, 60), ImVec2(0, 1), ImVec2(1, 0), 2, GetInspector()->IsSelected(asset), asset->m_name.c_str(), true)) {
				selected = n;
				switch (asset->m_type) {
					case EditorAssetType::FOLDER:
						m_currentFolder = (EditorAssetFolder*)asset;
						break;
					case EditorAssetType::SPRITE:

						break;
					case EditorAssetType::ANIMATION:
						break;
				}
				GetInspector()->SetSelected(asset);
			}

			//This needs a cleanup
			switch (asset->m_type) {
				case EditorAssetType::SPRITE: {
					static bool clicked = false;
					static bool dragging = false;
					static bool recording = false;
					static Vector2 mouseClickPos;
					static StreamedTexture* tex;
					if (ImGui::IsItemActive()) {
						if (!clicked) {
							clicked = true;
							mouseClickPos = GetMouse()->GetPosition();
						}
						if (dragging || mouseClickPos.Distance(GetMouse()->GetPosition()) > 25) {
							dragging = true;
							GameObject* obj = GetEditor()->GetGameObjectUnderMouse();
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
						}
					} else if (clicked && ImGui::IsMouseReleased(0)) {
						GameObject* obj = GetEditor()->GetGameObjectUnderMouse();
						dragging = false;
						clicked = false;
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