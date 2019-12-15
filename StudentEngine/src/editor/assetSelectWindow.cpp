#include "stdafx.h"

vector<AssetBase*> AssetSelectWindow::m_assetList;
vector<AssetBase*> AssetSelectWindow::m_searchResults;

AssetSelectWindow::AssetSelectWindow() {
	m_windowState = false;
}

void AssetSelectWindow::OnImGui() {
	if (m_windowState) {
		ImGui::SetNextWindowSize(ImVec2(450, 450), ImGuiCond_Once);
		ImGui::Begin("Select Asset", &m_windowState, ImGuiWindowFlags_NoDocking);

		// Search Bar
		static char m_searchBuffer[128];
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Search");
		ImGui::SameLine();
		ImGui::InputText("##SearchInput", m_searchBuffer, IM_ARRAYSIZE(m_searchBuffer), ImGuiInputTextFlags_CallbackAlways, UpdateResults);

		// Search Results
		ImGui::Text("Assets");
		ImGui::BeginChild("Assets");
		float windowWidth = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		ImVec2 buttonSize = ImVec2(128, 64);
		for (AssetBase* asset : m_searchResults) {
			if (ImGui::NamedButton((Texture*)asset->GetTexture(), buttonSize, ImVec2(0, 1), ImVec2(1, 0), 0, false, asset->GetFullFileName().c_str(), true)) {
				m_callback(asset);
				Close();
			}
			float buttonWidth = ImGui::GetItemRectMax().x;
			float nextButtonWidth = buttonWidth + buttonSize.x;
			if (nextButtonWidth < windowWidth) {
				ImGui::SameLine();
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
}

int AssetSelectWindow::UpdateResults(ImGuiTextEditCallbackData* inputData) {
	String searchInput = inputData->Buf;
	m_searchResults.clear();
	for (AssetBase* asset : m_assetList) {
		String assetName = asset->GetFullFileName();
		transform(searchInput.begin(), searchInput.end(), searchInput.begin(), [](unsigned char c) { return tolower(c); });
		transform(assetName.begin(), assetName.end(), assetName.begin(), [](unsigned char c) { return tolower(c); });
		if (assetName.find(searchInput) != string::npos) {
			m_searchResults.push_back(asset);
		}
	}
	return 0;
}

void AssetSelectWindow::Open() {
	if (!m_windowState) {
		m_windowState = true;
	}
}

void AssetSelectWindow::Close() {
	if (m_windowState) {
		m_windowState = false;
	}
}