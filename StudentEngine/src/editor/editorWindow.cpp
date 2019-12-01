#include "stdafx.h"

void editorWindow::Initialize() {
	// TODO: objs was what was used to initially develop the hierarchy interactions
	// Do we replace each 'objs' line with the rediculous long line or create a pointer/variable that references to it?
	objs = GetEditorManager()->GetHierarchy().m_gameObjects;

	//objs = vector<GameObject*>();
	objs.push_back(new GameObject("Player"));
	objs.push_back(new GameObject("Enemy"));
	objs.push_back(new GameObject("Camera"));
}
void editorWindow::End() {}

void editorWindow::OnImGui() {
	if (inEditorMode) {
		CreateDockingSpace();
		CreateEditorWindows();
	} else {

		CreateTemporaryPlayMode();
	}
}

void editorWindow::CreateTemporaryPlayMode() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::Button("Return to Edit")) {
			inEditorMode = true;
			//ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


void editorWindow::CreateDockingSpace() {
	//Create initial docking window
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Dock", nullptr, window_flags);

	ImGui::PopStyleVar(3);

	// Create docking space
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 1.5;
	ImGuiID dockspace_id;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		dockspace_id = ImGui::GetID("MyDockspace");

		if (!ImGui::DockBuilderGetNode(dockspace_id)) {
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_AutoHideTabBar);

			m_dockspaceCenter = dockspace_id;
			m_dockspaceRight = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Right, 0.3f, nullptr, &m_dockspaceCenter);
			m_dockspaceLeft = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Left, 0.3f, nullptr, &m_dockspaceCenter);
			m_dockspaceBottom = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Down, 0.2f, nullptr, &m_dockspaceCenter);
			m_dockspaceLeftBottom = ImGui::DockBuilderSplitNode(m_dockspaceLeft, ImGuiDir_Down, 0.4f, nullptr, &m_dockspaceLeft);

			ImGui::DockBuilderFinish(m_dockspaceCenter);
		}
		ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_NoDockingInCentralNode;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

	}
	ImGui::End();
}

void editorWindow::CreateEditorWindows() {
	// Menu Bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows")) {
			ImGui::EndMenu();
		}
		if (ImGui::Button("Enter Play Mode")) {
			inEditorMode = false;
			//ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10);

	static ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	// Main viewport
	ImGui::SetNextWindowDockID(m_dockspaceCenter, ImGuiCond_Always);
	if (ImGui::Begin("Editor Window###EditorWindow", nullptr, window_flags2)) {
		CreateViewport();
	}
	ImGui::End();

	// Hierarchy
	CreateSceneOverview(window_flags2);

	// Drag 'n Drop
	ImGui::SetNextWindowDockID(m_dockspaceLeftBottom, ImGuiCond_Always);

	//ImGuiContext* context = ImGui::GetCurrentContext();
	//context->FontBaseSize = 50;

	if (ImGui::Begin("Items", nullptr, window_flags2)) {
		if (ImGui::Button("Sprite", ImVec2(100,100))) {}
		ImGui::SameLine();
		if (ImGui::Button("GameObject", ImVec2(100, 100))) {}
		ImGui::SameLine();
		if (ImGui::Button("Scripts...", ImVec2(100, 100))) {}
		ImGui::SameLine();
		if (ImGui::Button("Stuff", ImVec2(100, 100))) {}
	}
	ImGui::End();

	// Throwing the logger in there for debugging purposes (for now)
	ImGui::SetNextWindowDockID(m_dockspaceBottom, ImGuiCond_Always);
	Logger::OnImGui();

	ImGui::PopStyleVar(3);
}

void editorWindow::CreateSceneOverview(ImGuiWindowFlags flags) {
	ImGui::SetNextWindowDockID(m_dockspaceLeft, ImGuiCond_Always);
	ImGui::Begin("Hierarchy", nullptr, flags);

	// TODO: Actual object types
	if (ImGui::TreeNode("Game Objects")) {
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 25);
		if (ImGui::Selectable("+"))
			AddItem();

		for (size_t i = 0; i < objs.size(); i++) {
			//ImGui::Selectable("\uf557" + objs[i].m_name.c_str());
			String title = ("\uf557 " + objs[i]->m_name);

			//Since all objects remain in the objs/hierachy class, we skip over those that have parents to prevent duplicates
			if (objs[i]->HasParent())
				continue;

			DisplaySceneChild(i, (objs[i]->m_children.size() > 0));
		}

		ImGui::TreePop();
	}
	ImGui::End();
}

void editorWindow::DisplaySceneChild(int index, bool hasChildren) {
	String title = ("\uf557 " + objs[index]->m_name);
	if (hasChildren) {
		for (size_t i = 0; i < objs[index]->m_children.size(); i++) {
			//title = ("\uf557 " + objs[index]->m_children[i]->m_name);
			if (ImGui::TreeNode(title.c_str())) {

				for (size_t child = 0; child < objs[index]->m_children.size(); child++) {
					if (objs[index]->HasParent())
						continue;
					// A convoluted way of finding the index of the child in the hierachy (and not in the child-list of the current object)
					vector<GameObject*>::iterator childIndexInHierarchy = find(objs.begin(), objs.end(), objs[index]->m_children[child]);
					int childIndex = distance(objs.begin(), childIndexInHierarchy);

					DisplaySceneChild(childIndex, (objs[childIndex]->m_children.size() > 0));
				}

				ImGui::TreePop();
			}
		}
	} else {
		if (ImGui::Selectable(title.c_str()))
			OnItemSelect(objs[index]);
	}

	//Right Click 
	if (ImGui::BeginPopupContextItem(nullptr, 1)) {
		if (settingNewParent) {
			if (ImGui::Selectable("Confirm Parent")) {
				SettingNewParent(index, childObjIndex);
			}
			if (ImGui::Selectable("Cancel Setting Parent")) {
				ToggleSettingNewParent();
			}
		} else {
			if (ImGui::Selectable("Delete")) {
				OnItemDelete(index);
			}
			if (ImGui::Selectable("Set parent")) {
				childObjIndex = index;
				ToggleSettingNewParent();
			}
		}
		/*if (ImGui::Selectable("Rename")) {
			ImGui::OpenPopup("Rename Item?");
			//OnItemRename(i);
		}*/

		ImGui::EndPopup();
	}

	//TODO: line 2628 in imgui_demo.cpp for debugging dragging
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75);
	ImGui::Selectable("\uf557 \uf557");
}

void editorWindow::OnItemSelect(GameObject* obj) {
	LOG("%s", obj->m_name.c_str());
}

void editorWindow::OnItemDelete(int index) {
	objs.erase(objs.begin() + index);
}

void editorWindow::OnItemRename(int index) {
}

void editorWindow::ToggleSettingNewParent() {
	settingNewParent = !settingNewParent;
}

void editorWindow::SettingNewParent(int parent, int child) {
	objs[parent]->AddChild(objs[child]);
	ToggleSettingNewParent();

	LOG("%i", objs[parent]->m_children.size());
}

void editorWindow::AddItem() {
	objs.push_back(new GameObject("New GameObject"));
}

void editorWindow::CreateViewport() {
	//fancy texture render pipeline here
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	ImVec2    win_region = ImGui::GetContentRegionAvail();
	ImVec2 w_pos = ImGui::GetCursorPos(), c_pos = ImGui::GetWindowPos();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	//#TODO Fix this mess
	//if (ImGui::GetCurrentWindowRead()->ParentWindow) {
		//ImVec2 parentPos = ImGui::GetCurrentWindowRead()->ParentWindow->Pos;

		//GetPipeline()->OnResize((uint)viewportSize.x, (uint)viewportSize.y);
	if (viewportSize.x > 0 && viewportSize.y > 0)
		GetFrameBufferManager()->OnResize((uint)viewportSize.x, (uint)viewportSize.y);
	//GetCamera()->UpdateProjectionMatrix();

	//Hardcoded 19 because we can't get this value from the parent window with ImGui::GetCurrentWindowRead()->ParentWindow->MenuBarHeight(); 
	GetPipeline()->m_camera->SetViewport(pos.x, pos.y, viewportSize.x, viewportSize.y);
	ImGui::Image((void*)GetPipeline()->GetFinalTexture()->GetHandle(), viewportSize, { 0, 1 }, { 1, 0 });
	//}
}
