#include "stdafx.h"

void HierarchyObject::OnImGui() {
	String title = Format("%s %s", ICON_FA_FOLDER, m_name.c_str());
	bool open = false;

	if (ImGui::TreeNode(title.c_str())) {
		open = true;
		RightClick(-1, true);
		FolderExtra();
		for (size_t c = 0; c < m_children.size(); c++) {
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
	if (GetEditorWindow()->m_dragPlacement && ImGui::IsItemHovered()) {
		GetEditorWindow()->MoveToFolder(this);
		//TODO fklgdfjkgdfhkgdfgkjhg
		GetEditorWindow()->m_dragPlacement = false;
	}

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75);
	ImGui::Selectable(Format_t("%s", ICON_FA_EYE));
}

void HierarchyObject::DisplayChild(int index) {
	GameObject* child = m_children[index];
	String title = Format("%s %s", ICON_FA_BOX, child->m_name.c_str()); 

	bool selected = (GetEditorManager()->GetHierarchy().GetSelected() == child);
	if (ImGui::Selectable(title.c_str(), selected))
		OnItemSelect(child);

	RightClick(index, false);
	GuiItemDrag(index);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75);
	ImGui::Selectable(Format_t("%s", ICON_FA_EYE));
}


void HierarchyObject::OnItemSelect(GameObject* obj) {
	//LOG("%s", obj->m_name.c_str());
	GetEditorManager()->GetHierarchy().SetSelected(obj);
	GetInspector()->SetSelected(obj);
}

void HierarchyObject::RightClick(int index, bool folder) {
	if (ImGui::BeginPopupContextItem(nullptr, 1)) {
		if (folder) {
			if (ImGui::Selectable("Confirm Folder")) {
				GetEditorWindow()->MoveToFolder(this);
			}
			if (ImGui::Selectable("Cancel Setting Parent")) {
				GetEditorWindow()->ToggleSettingNewParent(NULL);
			}
		}
		else {
			//Automatically preventing the player to set multiple parents at the same time
			if (ImGui::Selectable("Change Folder") && !GetEditorWindow()->SettingNewFolder()) {
				GetEditorWindow()->ToggleSettingNewParent(m_children[index]);
			}
			// TODO: Reimplement deleting
			/*if (ImGui::Selectable("Delete")) {
				//OnItemDelete(index);
			}*/
		}

		ImGui::EndPopup();
	}
}

void HierarchyObject::GuiItemDrag(int index) {
	if (ImGui::IsItemActive()) {
		GetEditorWindow()->m_draggingItem = true;
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
	//gameObject->SetParent(this);
	m_children.push_back(gameObject);
}

void HierarchyObject::RemoveChild(GameObject* gameObject) {
	m_children.erase(remove(m_children.begin(), m_children.end(), gameObject), m_children.end());
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