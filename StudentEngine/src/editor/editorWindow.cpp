#include "stdafx.h"

void editorWindow::Initialize() {
	// TODO: objs was what was used to initially develop the hierarchy interactions
	// Do we replace each 'objs' line with the rediculous long line or create a pointer/variable that references to it?
	objs = GetEditorManager()->GetHierarchy().m_gameObjects;
}

void editorWindow::Update(const TimeStep& time) {

}

void editorWindow::Draw() {

}

void editorWindow::OnImGui() {
	CreateDockingSpace();
	if (inEditorMode) {
		CreateEditorWindows();
	} else {
		CreateTemporaryPlayMode();
	}
}

void editorWindow::CreateTemporaryPlayMode() {
	ImGui::SetNextWindowDockID(m_dockspaceCenter, ImGuiCond_Always);
	ImGuiWindowFlags playwindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
	if (ImGui::Begin("Play Window"), nullptr, playwindowFlags) {
		if (ImGui::IsWindowFocused()) {
			ImGui::GetIO().WantCaptureMouse = false;
			ImGui::GetIO().WantCaptureKeyboard = false;
		}

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::Button("Return to Edit")) {
				inEditorMode = true;
				GetStateManager()->SetState(States::EDIT);
			}
		}
		ImGui::EndMainMenuBar();
		CreateViewport();
	}
	ImGui::End();
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

	m_mainWindowPos = ImGui::GetWindowPos();
	ImGui::PopStyleVar(3);

	// Create docking space
	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id;

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
			GetStateManager()->SetState(States::PLAY);
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
		Undo::OnImGui();
		if (ImGui::IsWindowFocused()) {
			ImGui::GetIO().WantCaptureMouse = false;
			ImGui::GetIO().WantCaptureKeyboard = false;
		}
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
		if (ImGui::Button("Sprite", ImVec2(100, 100))) {}
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
			//Since all objects remain in the objs/hierachy class, we skip over those that have parents to prevent duplicates
			if (objs[i]->HasParent())
				continue;

			DisplaySceneChild(i, ((int)objs[i]->GetChildren().size() > 0));
		}

		ImGui::TreePop();
	}
	ImGui::End();
}

void editorWindow::DisplaySceneChild(int index, bool hasChildren) {
	String title = Format("%s %s", ICON_FA_CAMERA, objs[index]->m_name.c_str());
	if (hasChildren) {
		for (size_t i = 0; i < objs[index]->GetChildren().size(); i++) {
			if (ImGui::TreeNode(title.c_str())) {
				for (size_t child = 0; child < objs[index]->GetChildren().size(); child++) {
					// A convoluted way of finding the index of the child in the hierachy (and not in the child-list of the current object)
					vector<GameObject*>::iterator childIndexInHierarchy = find(objs.begin(), objs.end(), objs[index]->GetChildren()[child]);
					int childIndex = distance(objs.begin(), childIndexInHierarchy);

					DisplaySceneChild(childIndex, (objs[childIndex]->GetChildren().size() > 0));
				}

				ImGui::TreePop();
			}
		}
	} else {
		//Bool that controls which object in the hierachy gets highlighted. Since there's only one selected object in the hierarchy class, that's straightforward to do
		bool selected = (GetEditorManager()->GetHierarchy().m_selected == objs[index]);
		if (ImGui::Selectable(title.c_str(), selected))
			OnItemSelect(objs[index]);
	}

	//Right Click 
	if (ImGui::BeginPopupContextItem(nullptr, 1)) {
		if (settingNewParent) {
			// We check whether the selected obj is the to-be child && if they already are a child of that GameObject && if they're going to eachother's parent
			// If all of these are false, then we'll have a normal, functioning hierachy
			bool canSetParent = (index != childObjIndex && !objs[index]->ContainsChild(objs[childObjIndex]) && !(objs[index]->GetParent() == objs[childObjIndex]));
			if (canSetParent && ImGui::Selectable("Confirm Parent")) {
				SettingNewParent(index, childObjIndex);
			}
			if (ImGui::Selectable("Cancel Setting Parent")) {
				ToggleSettingNewParent();
			}
		} else {
			if (ImGui::Selectable("Set parent")) {
				//Using a class variable so we can use it when the user confirms the parent (by clicking on the parent, so we need to remember the child)
				childObjIndex = index;
				ToggleSettingNewParent();
			}
			if (ImGui::Selectable("Delete")) {
				//OnItemDelete(index);
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
	//LOG("%s", obj->m_name.c_str());
	GetEditorManager()->GetHierarchy().m_selected = obj;
}

void editorWindow::OnItemDelete(int index) {
	objs.erase(objs.begin() + index);
	//GetEditorManager()->GetHierarchy().
}

void editorWindow::OnItemRename(int index) {
}

void editorWindow::ToggleSettingNewParent() {
	settingNewParent = !settingNewParent;
}

void editorWindow::SettingNewParent(int parent, int child) {
	if (objs[child]->HasParent()) {
		Undo::Record(objs[child]->GetParent());

		objs[child]->GetParent()->RemoveChild(objs[child]);
		objs[child]->SetParent(NULL);
	}

	Undo::Record(objs[child]);
	objs[child]->SetParent(objs[parent]);

	Undo::FinishRecording();

	Undo::Record(objs[parent]);
	objs[parent]->AddChild(objs[child]);
	ToggleSettingNewParent();

	Undo::FinishRecording();
}

void editorWindow::AddItem() {
	//objs.push_back(new GameObject("New GameObject"));

	// TODO: Copied from editorManager, nice to show off during Monday but that's it!
	StreamedTexture* logo;
	logo = GetAssetManager()->Get<StreamedTexture>("Logo");
	String name = Format("Object %i", GetEditorManager()->GetHierarchy().m_gameObjects.size()+1);
	GetEditorManager()->AddGameObject(new GameObject(name))
		.SetSize(Vector2(500, 500))
		.SetPosition(Vector2(300.0f, GetApp()->GetPipeline()->m_camera->GetRelativeViewport().w / 2))
		.SetTexture(logo);


	objs = GetEditorManager()->GetHierarchy().m_gameObjects;
}

void editorWindow::CreateViewport() {
	//if (GetStateManager()->GetState() == States::LOADING) return;
	ImGuiViewport* viewport = ImGui::GetWindowViewport();
	ImGuiContext& g = *GImGui;

	ImVec2    actualWindowSize = ImGui::GetContentRegionAvail();
	ImVec2    actualWindowPosition = ImGui::GetCursorScreenPos();
	actualWindowPosition.x -= m_mainWindowPos.x;
	actualWindowPosition.y -= m_mainWindowPos.y;

	m_viewport = Vector4(actualWindowPosition.x, actualWindowPosition.y, actualWindowSize.x, actualWindowSize.y);
	
	ImGui::Image((void*)GetApp()->GetPipeline()->GetFinalTexture()->GetHandle(), actualWindowSize, { 0, 1 }, { 1, 0 });
}