#include "stdafx.h"

void editorWindow::Initialize() {
	// TODO: objs was what was used to initially develop the hierarchy interactions
	// Do we replace each 'objs' line with the rediculous long line or create a pointer/variable that references to it?
	m_folders = vector<HierarchyObject*>();
	//m_hierarchyyyy = vector<HierarchyObject*>();
	//m_folders.push_back(GetEditorManager()->GetHierarchy());
	m_folders.push_back(new HierarchyObject("Folder - Background"));
	m_folders.push_back(new HierarchyObject("Folder - Items"));

	m_folders[0]->AddChild(GetEditorManager()->GetHierarchy().m_gameObjects[0]);
	m_folders[0]->AddChild(GetEditorManager()->GetHierarchy().m_gameObjects[1]);
	////GetEditorManager()->GetHierarchy().m_gameObjects[0]->

	SetupEditorStyle(true, 0.5f);
}

void editorWindow::Update(const TimeStep& time) {
	if (m_mouseInViewport)GetCamera()->UpdateZoom(time);
}

void editorWindow::Draw() {

}

void editorWindow::OnImGui() {
	CreateDockingSpace();
	if (m_inEditorMode) {
		CreateEditorWindows();
	} else {
		CreateTemporaryPlayMode();
	}
}

void editorWindow::CreateTemporaryPlayMode() {
	ImGui::SetNextWindowDockID(m_dockspaceCenter, ImGuiCond_Always);
	ImGuiWindowFlags playwindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
	if (ImGui::Begin("Play Window"), nullptr, playwindowFlags) {
		if (m_mouseInViewport) {
			ImGui::GetIO().WantCaptureMouse = false;
			ImGui::GetIO().WantCaptureKeyboard = false;
		}

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::Button("Return to Edit")) {
				m_inEditorMode = true;
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
			m_dockspaceRight = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Right, 0.2f, nullptr, &m_dockspaceCenter);
			m_dockspaceLeft = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Left, 0.2968725f, nullptr, &m_dockspaceCenter);
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
			m_inEditorMode = false;
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
		if (m_mouseInViewport) {
			ImGui::GetIO().WantCaptureMouse = false;
			ImGui::GetIO().WantCaptureKeyboard = false;
		}
		CreateViewport();
	}
	ImGui::End();

	//Response to Dragging
	if (ImGui::IsMouseReleased(0) && m_draggingItem) {
		m_draggingItem = false;

		Vector2 pos = GetMouse()->GetPosition();
		bool inWindow = Math::Within(pos.x, m_viewport.x, m_viewport.x + m_viewport.z) && Math::Within(pos.y, m_viewport.y, m_viewport.y + m_viewport.w);

		if (inWindow) {
			Vector3 ray = GroundRaycast::GetMousePosition(GetCamera());
			Vector3 rayPos = GroundRaycast::GetGroundPosition(GetCamera(), ray, 1.0f);

			// #TODO: Add different responses to each draggable button!
			AddItem(Vector2(rayPos.x, rayPos.y));
			GetEditorManager()->GetHierarchy().m_selected = GetEditorManager()->GetHierarchy().m_gameObjects[GetEditorManager()->GetHierarchy().m_gameObjects.size() - 1];
		} else {
			//Setting a bool that gets picked up on the ImGui on folders later in the same loop/frame
			m_dragPlacement = true;
		}
	}

	ImGui::PopStyleVar(3);

	CreateSceneOverview(window_flags2);

	// Drag 'n Drop
	ImGui::SetNextWindowDockID(m_dockspaceLeftBottom, ImGuiCond_Always);
	if (ImGui::Begin("Items", nullptr, window_flags2)) {
		if (ImGui::Button("Sprite", ImVec2(100, 100))) {}
		CreateItemDrag();
		ImGui::SameLine();
		if (ImGui::Button("GameObject", ImVec2(100, 100))) {}
		CreateItemDrag();
		ImGui::SameLine();
		if (ImGui::Button("Scripts...", ImVec2(100, 100))) {}
		CreateItemDrag();
		ImGui::SameLine();
		if (ImGui::Button("Stuff", ImVec2(100, 100))) {}
		CreateItemDrag();
	}
	ImGui::End();


	// Throwing the logger in there for debugging purposes (for now)
	ImGui::SetNextWindowDockID(m_dockspaceBottom, ImGuiCond_Always);
	GetEditorAssetManager()->OnImGui();

	ImGui::SetNextWindowDockID(m_dockspaceRight, ImGuiCond_Always);
	GetInspector()->OnImGui();

}

void editorWindow::CreateItemDrag() {
	if (ImGui::IsItemActive()) {
		m_draggingItem = true;
		ImGuiIO io = ImGui::GetIO();
		// Draw a line between the button and the mouse cursor
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->PushClipRectFullScreen();
		draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
		//draw_list->AddText(ImVec2(io.MousePos.x, io.MousePos.y - 20), ImU32("#ffffff"), "Object");
		draw_list->PopClipRect();
	}
}

void editorWindow::CreateSceneOverview(ImGuiWindowFlags flags) {
	ImGui::SetNextWindowDockID(m_dockspaceLeft, ImGuiCond_Always);
	ImGui::Begin("Hierarchy", nullptr, flags);

	for (size_t i = 0; i < m_folders.size(); i++) {
		DisplayFolder(i, ((int)m_folders[i]->GetChildren().size() > 0));
	}
	ImGui::End();
}

void editorWindow::DisplayFolder(int index, bool hasChildren) {
	HierarchyObject* currFolder = m_folders[index];
	//vector<GameObject*> hierarchyGameObjs = GetEditorManager()->GetHierarchy().m_gameObjects;

	currFolder->OnImGui();

	//GuiItemDrag();

	//Right Click 
	/*if (ImGui::BeginPopupContextItem(nullptr, 1)) {
		if (m_settingNewParent) {
			// We check whether the selected obj is the to-be child && if they already are a child of that GameObject && if they're going to eachother's parent
			// If all of these are false, then we'll have a normal, functioning hierachy
			bool canSetParent = (index != m_childObjIndex && !m_folders[index]->ContainsChild(m_folders[m_childObjIndex]) && !(m_folders[index]->GetParent() == m_folders[m_childObjIndex]));
			if (canSetParent && ImGui::Selectable("Confirm Parent")) {
				SettingNewParent(index, m_childObjIndex);
			}
			if (ImGui::Selectable("Cancel Setting Parent")) {
				ToggleSettingNewParent();
			}
		} else {
			if (ImGui::Selectable("Set parent")) {
				//Using a class variable so we can use it when the user confirms the parent (by clicking on the parent, so we need to remember the child)
				m_childObjIndex = index;
				ToggleSettingNewParent();
			}
			if (ImGui::Selectable("Delete")) {
				//OnItemDelete(index);
			}
		}
		/*if (ImGui::Selectable("Rename")) {
			ImGui::OpenPopup("Rename Item?");
			//OnItemRename(i);
		}

		ImGui::EndPopup();
	}*/

	
}

void editorWindow::DisplayObject(GameObject* obj) {
	String title = Format("%s %s", ICON_FA_BOX, obj->m_name.c_str());

	bool selected = (GetEditorManager()->GetHierarchy().m_selected == obj);
	//if (ImGui::Selectable(title.c_str(), selected))
	//	OnItemSelect(obj);


	//GuiItemDrag();
}

/*void editorWindow::OnItemSelect(GameObject* obj) {
	//LOG("%s", obj->m_name.c_str());
	GetEditorManager()->GetHierarchy().m_selected = obj;
}*/

void editorWindow::OnItemDelete(int index) {
	m_folders.erase(m_folders.begin() + index);
	//GetEditorManager()->GetHierarchy().
}

void editorWindow::OnItemRename(int index) {

}

void editorWindow::MoveToFolder(HierarchyObject* folder) {
	HierarchyObject* oldFolder = FindFolderOfObject(m_movingChild);
	//TODO: Prevents error that needs cleaning up!
	if (oldFolder == NULL)
		return;

	oldFolder->RemoveChild(m_movingChild);
	folder->AddChild(m_movingChild);
	m_settingNewFolder = false;
}

HierarchyObject* editorWindow::FindFolderOfObject(GameObject* obj) {
	for each (HierarchyObject* folder in m_folders) {
		if (folder->ContainsChild(obj)) {
			return folder;
		}
	}

	return NULL;
}

void editorWindow::ToggleSettingNewParent(GameObject* obj = NULL) {
	if (obj != NULL)
		m_movingChild = obj;

	m_settingNewFolder = !m_settingNewFolder;
}

/*void editorWindow::SettingNewParent(int parent, int child) {
	if (m_folders[child]->HasParent()) {
		Undo::Record(m_folders[child]->GetParent());

		m_folders[child]->GetParent()->RemoveChild(m_folders[child]);
		m_folders[child]->SetParent(NULL);
	}

	Undo::Record(m_folders[child]);
	m_folders[child]->SetParent(m_folders[parent]);

	Undo::FinishRecording();

	Undo::Record(m_folders[parent]);
	m_folders[parent]->AddChild(m_folders[child]);
	ToggleSettingNewParent();

	Undo::FinishRecording();
}*/

void editorWindow::AddItem(Vector2 pos = NULL) {
	if (pos == NULL) {
		pos = Vector2(300.0f, GetCamera()->GetRelativeViewport().w / 2);
	}
	//objs.push_back(new GameObject("New GameObject"));

	// TODO: Copied from editorManager, nice to show off during Monday but that's it!
	StreamedTexture* logo;
	logo = GetAssetManager()->Get<StreamedTexture>("Logo");
	String name = Format("Object %i", GetEditorManager()->GetHierarchy().m_gameObjects.size() + 1);
	GetEditorManager()->AddGameObject(new GameObject(name))
		.SetSize(Vector2(500, 500))
		.SetPosition(pos)
		.SetTexture(logo);

	m_folders[0]->AddChild(GetEditorManager()->GetHierarchy().m_gameObjects[GetEditorManager()->GetHierarchy().m_gameObjects.size()-1]);

	//m_folders = GetEditorManager()->GetHierarchy().m_gameObjects;
}

void editorWindow::AddFolder() {

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
	m_mouseInViewport = ImGui::IsWindowHovered(ImGuiHoveredFlags_RectOnly);
	ImGui::Image((void*)GetApp()->GetPipeline()->GetFinalTexture()->GetHandle(), actualWindowSize, { 0, 1 }, { 1, 0 });
}

void editorWindow::SetupEditorStyle(bool bStyleDark, float alpha) {
	ImGuiStyle& style = ImGui::GetStyle();

	// light style from Pacôme Danhiez (user itamago)
	// https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
	style.Alpha = 1.0f;
	//style.FrameRounding = 1.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.84f, 0.84f, 0.84f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.98f, 0.59f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.98f, 0.59f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	/*style.Colors[ImGuiCol_ComboBg]			= ImVec4(0.86f, 0.86f, 0.86f, 0.99f);*/
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.98f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.98f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.88f, 0.52f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.98f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.98f, 0.59f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.98f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.87f, 0.53f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.98f, 0.59f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.98f, 0.59f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.98f, 0.59f, 1.00f);

	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.95f, 0.59f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.95f, 0.59f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.65f, 0.65f, 0.65f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.95f, 0.59f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.95f, 0.59f, 0.95f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.98f, 0.59f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.26f, 0.98f, 0.59f, 0.35f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.98f, 0.59f, 0.50f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.98f, 0.59f, 0.60f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.26f, 0.98f, 0.59f, 0.25f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.26f, 0.98f, 0.59f, 0.40f);


	style.WindowRounding = 5;
	style.FrameRounding = 2;
	style.TabRounding = 5;
	style.ChildRounding = 5;
	style.PopupRounding = 5;

	if (bStyleDark) {
		for (int i = 0; i <= ImGuiCol_COUNT; i++) {
			ImVec4& col = style.Colors[i];
			float H, S, V;
			ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

			if (S < 0.1f) {
				V = 1.0f - V;
			}
			ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
			if (col.w < 1.00f) {
				col.w *= alpha;
			}
		}
	} else {
		for (int i = 0; i <= ImGuiCol_COUNT; i++) {
			ImVec4& col = style.Colors[i];
			if (col.w < 1.00f) {
				col.x *= alpha;
				col.y *= alpha;
				col.z *= alpha;
				col.w *= alpha;
			}
		}
	}
}