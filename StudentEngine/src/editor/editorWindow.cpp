#include "stdafx.h"

void editorWindow::Initialize() {
	// TODO: objs was what was used to initially develop the hierarchy interactions
	// Do we replace each 'objs' line with the rediculous long line or create a pointer/variable that references to it?
	objs = GetEditorManager()->GetHierarchy().m_gameObjects;

	ImGuiIO& io = ImGui::GetIO();
	io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/Consolas.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesDefault());
	
	// Get the icons from both FA regular & solid (each having their own range of different icons)
	// And add them to the already existing font
	ImFontConfig config;
	config.MergeMode = true;
	const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromFileTTF("res/fonts/fa-regular-400.ttf", 15.0f, &config, icon_ranges);
	io.Fonts->AddFontFromFileTTF("res/fonts/fa-solid-900.ttf", 15.0f, &config, icon_ranges);
	io.Fonts->Build();
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
				GetStateManager()->SetState<EditState>();
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
	//io.FontGlobalScale = 1.5;
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
			GetStateManager()->SetState<PlayState>();
		}
		ImGui::Text("%s Search", ICON_FA_SMILE);
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

	//ImGui::End();
	//
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
			//ImGui::Selectable("\uf557" + objs[i].m_name.c_str());
			//String title = (ICON_FA_CUBE + objs[i]->m_name);

			//Since all objects remain in the objs/hierachy class, we skip over those that have parents to prevent duplicates
			/*if (objs[i]->HasParent())
				continue;*/

			DisplaySceneChild(i, (objs[i]->GetChildren().size() > 0));
		}

		ImGui::TreePop();
	}
	ImGui::End();
}

void editorWindow::DisplaySceneChild(int index, bool hasChildren) {
	String title = (ICON_FA_CAMERA + String(" ") + objs[index]->m_name);
	if (hasChildren) {
		for (size_t i = 0; i < objs[index]->GetChildren().size(); i++) {
			//title = ("\uf557 " + objs[index]->m_children[i]->m_name);
			if (ImGui::TreeNode(title.c_str())) {

				for (size_t child = 0; child < objs[index]->GetChildren().size(); child++) {
					if (objs[index]->HasParent())
						continue;
					// A convoluted way of finding the index of the child in the hierachy (and not in the child-list of the current object)
					vector<GameObject*>::iterator childIndexInHierarchy = find(objs.begin(), objs.end(), objs[index]->GetChildren()[child]);
					int childIndex = distance(objs.begin(), childIndexInHierarchy);

					DisplaySceneChild(childIndex, (objs[childIndex]->GetChildren().size() > 0));
				}

				ImGui::TreePop();
			}
		}
	} else {
		bool selected = (GetEditorManager()->GetHierarchy().m_selected == objs[index]);
		if (ImGui::Selectable(title.c_str(), selected))
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
	objs[parent]->AddChild(objs[child]);
	ToggleSettingNewParent();

	//LOG("%i", objs[parent]->m_children.size());
}

void editorWindow::AddItem() {
	//objs.push_back(new GameObject("New GameObject"));

	// TODO: Copied from editorManager, nice to show off during Monday but that's it!
	Texture* logo;
	logo = GetAssetManager()->Get<Texture>("Logo");
	String name = "Object" + (GetEditorManager()->GetHierarchy().m_gameObjects.size());
	GetEditorManager()->AddGameObject(new GameObject(name))
		.SetSize(Vector2(500, 500))
		.SetPosition(Vector2(300.0f, GetApp()->GetPipeline()->m_camera->GetRelativeViewport().w / 2))
		.SetTexture(logo);


	objs = GetEditorManager()->GetHierarchy().m_gameObjects;
}

void editorWindow::CreateViewport() {
	ImGuiViewport* viewport = ImGui::GetWindowViewport();
	ImGuiContext& g = *GImGui;

	ImVec2    actualWindowSize = ImGui::GetContentRegionAvail();
	ImVec2    actualWindowPosition = ImGui::GetCursorScreenPos();
	actualWindowPosition.x -= m_mainWindowPos.x;
	actualWindowPosition.y -= m_mainWindowPos.y;

	ImGui::Image((void*)GetApp()->GetPipeline()->GetFinalTexture()->GetHandle(), actualWindowSize, { 0, 1 }, { 1, 0 });
	
	if (viewport->Size.x > 0 && viewport->Size.y > 0)
		GetFrameBufferManager()->OnResize((uint)actualWindowSize.x, (uint)actualWindowSize.y);

	GetApp()->GetPipeline()->m_camera->SetViewport(actualWindowPosition.x, actualWindowPosition.y, actualWindowSize.x, actualWindowSize.y);
}
