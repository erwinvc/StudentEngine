#include "stdafx.h"

void HierarchyObject::OnImGui() {
	String title = Format("%s %s", ICON_FA_FOLDER, m_name.c_str());
	bool open = false;

	if (ImGui::TreeNode(title.c_str())) {
		open = true;
		RightClick(-1, true);
		FolderExtra();
		for (int c = 0; c < m_children.size(); c++) {
			DisplayChild(c);
		}

		ImGui::TreePop();
	}

	//To allow for item dragging & to prevent double icon from appearing over a folder
	if (!open) {
		FolderExtra();
	}
}

void HierarchyObject::FolderExtra() {
	if (GetEditorWindow()->IsInDragPlacement() && ImGui::IsItemHovered()) {
		Undo::Record(GetEditorWindow()->m_movingChild);
		GetEditorWindow()->MoveToFolder(this, nullptr);
		GetEditorWindow()->SetDragPlacement(false);
		Undo::FinishRecording();
	}

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75);
	ImGui::Selectable(Format_t("%s", ICON_FA_EYE));
}

void HierarchyObject::DisplayChild(int index) {
	GameObject* child = m_children[index];
	String title = Format("%s %s", ICON_FA_BOX, child->m_name.c_str()); 

	bool selected = (GetScene()->GetHierarchy().GetSelected() == child);
	if (ImGui::Selectable(title.c_str(), selected))
		OnItemSelect(child);

	RightClick(index, false);
	GuiObjectDrag(index);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75);
	ImGui::Selectable(Format_t("%s", ICON_FA_EYE));
}


void HierarchyObject::OnItemSelect(GameObject* obj) {
	GetScene()->GetHierarchy().SetSelected(obj);
	GetInspector()->SetSelected(obj);
}

void HierarchyObject::RightClick(int index, bool folder) {
	if (ImGui::BeginPopupContextItem(nullptr, 1)) {
		if (!folder) {
			if (ImGui::Button("Delete")) {
				OnItemDelete(index);
			}

			if (ImGui::Button("Rename Item")) {
				//We copy the name here into our helper variable to prevent it from reapplying it every frame
				strcpy(m_renameHelper, m_children[index]->m_name.c_str());
				ImGui::OpenPopup("Rename Item");
			}

			if (ImGui::BeginPopupModal("Rename Item", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Enter name here\n\n");
				ImGui::Separator();
				ImGui::InputText("", m_renameHelper, IM_ARRAYSIZE(m_renameHelper));
				if (ImGui::Button("OK", ImVec2(120, 0))) { 
					ImGui::CloseCurrentPopup();
					m_children[index]->m_name = Format("%s", m_renameHelper);
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
		}

		ImGui::EndPopup();
	}
}

void HierarchyObject::GuiObjectDrag(int index) {
	if (ImGui::IsItemActive()) {
		GetEditorWindow()->InstantiateDragging(true);
		GetEditorWindow()->ToggleSettingNewParent(m_children[index]);
		ImGuiIO io = ImGui::GetIO();
		// Draw a line between the button and the mouse cursor
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->PushClipRectFullScreen();
		draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
		//draw_list->AddText(ImVec2(io.MousePos.x, io.MousePos.y - 20), ImU32("#ffffff"), "Object");
		draw_list->PopClipRect();
	}
}

void HierarchyObject::AddChild(GameObject* gameObject) {
	m_children.push_back(gameObject);
}

void HierarchyObject::RemoveChild(GameObject* gameObject) {
	m_children.erase(remove(m_children.begin(), m_children.end(), gameObject), m_children.end());
}

void HierarchyObject::OnItemDelete(int index) {
	GetScene()->GetHierarchy().DeleteGameObject(m_children[index]);

	RemoveChild(m_children[index]);
}

vector<GameObject*> HierarchyObject::GetChildren() {
	return m_children;
}

GameObject* HierarchyObject::GetChildAt(int index) {
	return m_children[index];
}

bool HierarchyObject::ContainsChild(GameObject* child) {
	bool foundChild = false;
	for (size_t i = 0; i < m_children.size(); i++) {
		foundChild = m_children[i] == child;
	}
	return foundChild;
}