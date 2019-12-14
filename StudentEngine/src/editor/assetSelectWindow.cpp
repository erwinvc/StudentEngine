#include "stdafx.h"

AssetSelectWindow::AssetSelectWindow() {
	m_windowState = false;
}

void AssetSelectWindow::OnImGui() {
	if (m_windowState) {
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
		ImGui::Begin("Select Asset", &m_windowState, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoDocking);

		// Search Bar
		static char m_searchBuffer[128];
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Search");
		ImGui::SameLine();
		ImGui::InputText("##SearchInput", m_searchBuffer, IM_ARRAYSIZE(m_searchBuffer), ImGuiInputTextFlags_CallbackCharFilter, UpdateResults);

		// Search Results
		ImGui::Text("Assets");
		ImGui::BeginChild("Assets");
		for (AssetBase* asset : m_assetList) {
			Path currentPath = asset->GetFilePath();
			if (ImGui::Button((currentPath.GetFileName() + "." + currentPath.GetExtention()).c_str())) {
				m_callback(asset);
				Close();
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
}

int AssetSelectWindow::UpdateResults(ImGuiTextEditCallbackData* inputData) {
	// TODO: Update search results
	return 0;
}

void AssetSelectWindow::Open() {
	m_windowState = true;
}

void AssetSelectWindow::Close() {
	m_windowState = false;
}