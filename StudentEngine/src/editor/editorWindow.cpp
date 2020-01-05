#include "stdafx.h"


void EditorWindow::Initialize() {
	m_layers = vector<HierarchyObject*>();
	for (auto& layer : GetEditorScene()->GetHierarchy().m_layers) {
		m_layers.push_back(new HierarchyObject(layer));
	}

	SetupEditorStyle(true, 0.5f);
}

EditorWindow::~EditorWindow() {
	for (auto& folder : m_layers) {
		delete folder;
	}
	m_layers.clear();
}

void EditorWindow::Update(const TimeStep& time) {
	if (m_mouseInViewport)GetCamera()->UpdateZoom(time);
}

void EditorWindow::Draw() {

}

void EditorWindow::OnImGui() {
	CreateDockingSpace();
	if (m_inEditorMode) {
		CreateEditorWindows();
	} else {
		CreateTemporaryPlayMode();
	}
}

void EditorWindow::CreateTemporaryPlayMode() {
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

void EditorWindow::CreateDockingSpace() {
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
			m_dockspaceLeft = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Left, 0.15f, nullptr, &m_dockspaceCenter);
			m_dockspaceBottom = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Down, 0.2f, nullptr, &m_dockspaceCenter);
			m_dockspaceUp = ImGui::DockBuilderSplitNode(m_dockspaceCenter, ImGuiDir_Up, 0.16f, nullptr, &m_dockspaceCenter);
			m_dockspaceLeftBottom = ImGui::DockBuilderSplitNode(m_dockspaceLeft, ImGuiDir_Down, 0.4f, nullptr, &m_dockspaceLeft);

			ImGui::DockBuilderFinish(m_dockspaceCenter);
		}
		ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_NoDockingInCentralNode;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

	}
	ImGui::End();
}

void EditorWindow::CreateEditorWindows() {
	// Menu Bar
	/*if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				GetApp()->OnWindowClose();
			}
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
	}*/

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10);

	static ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	// Main viewport
	ImGui::SetNextWindowDockID(m_dockspaceCenter, ImGuiCond_Always);
	if (ImGui::Begin("Editor Window###EditorWindow", nullptr, window_flags2)) {
		//Undo::OnImGui();
		if (m_mouseInViewport) {
			ImGui::GetIO().WantCaptureMouse = false;
			ImGui::GetIO().WantCaptureKeyboard = false;
		}
		CreateViewport();
	}
	ImGui::End();

	// BUTTONS TEST

	ImGui::SetNextWindowDockID(m_dockspaceUp, ImGuiCond_Always);
	ImGui::Begin("Buttons", nullptr, window_flags2);

	ImFont* bigIconFont = ImGui::GetIO().Fonts->Fonts[1];
	ImGui::PushFont(bigIconFont);
	if (ImGui::Button(ICON_FA_PLAY)) {
		m_inEditorMode = false;
		GetStateManager()->SetState(States::PLAY);
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FOLDER_OPEN)) {

	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_SAVE)) {

	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_LAYER_GROUP)) {
		m_openedLayerManager = !m_openedLayerManager;
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_SEARCH)) {
		m_openedInspector = !m_openedInspector;
	}
	ImGui::SameLine();

	ImGui::PopFont();

	ImGui::End();

	// BUTTONS TEST

	//Response to Dragging
	if (ImGui::IsMouseReleased(0) && m_draggingItem) {
		m_draggingItem = false;

		Vector2 pos = GetMouse()->GetPosition();
		bool inWindow = Math::Within(pos.x, m_viewport.x, m_viewport.x + m_viewport.z) && Math::Within(pos.y, m_viewport.y, m_viewport.y + m_viewport.w);
		
		//Currently whenever the drag source (in hierachyObject) is external, that means we aren't creating an object, just moving folders
		if (inWindow && !m_dragSourceExternal) {
			Vector3 ray = GroundRaycast::GetMousePosition(GetCamera());
			Vector3 rayPos = GroundRaycast::GetGroundPosition(GetCamera(), ray, 1.0f);

			String name = Format("Object %i", GetEditorScene()->GetHierarchy().Size() + 1);
			GameObject& obj = GetEditorScene()->AddGameObject(new GameObject(name, false))
				.SetSize(Vector2(128, 128))
				.SetPosition(Math::RoundToNumber(Vector2(rayPos), Vector2(32.0f, 32.0f)));

			switch (m_currentlyDraggedEditorObjectType) {
				case EditorObjectType::TERRAIN:
					obj.SetTexture(GetAssetManager()->Get<StreamedTexture>("9slice")).Set9Slice();
					GetEditorScene()->GetHierarchy().ChangeLayer(&obj, "Background");
					break;
				case EditorObjectType::GAMEOBJECT:
					obj.SetTexture(GetAssetManager()->Get<StreamedTexture>("Logo"));
					break;
			}

			//m_layers[0]->AddChild(&obj);
			GetEditorScene()->GetHierarchy().SetSelected(&obj);
		} else {
			//Setting a bool that gets picked up on the ImGui on folders later in the same loop/frame
			m_dragPlacement = true;
		}
	}

	ImGui::PopStyleVar(3);

	if (m_openedLayerManager)
		CreateSceneOverview(window_flags2);

	// Drag 'n Drop
	
	ImGui::SetNextWindowDockID(m_dockspaceLeft, ImGuiCond_Always);
	if (ImGui::Begin("Items", nullptr, window_flags2)) {

		//TODO: Possibly loop through the Enum?
		/*for (EditorObjectType type = EditorObjectType::GAMEOBJECT; type != EditorObjectType::TERRAIN; type = EditorObjectType(type+1)) {
		
		}*/
		
		if (ImGui::Button("Terrain", ImVec2(100, 100))) {}
		CreateItemDrag(EditorObjectType::TERRAIN);
		ImGui::SameLine();
		if (ImGui::Button("Sprite", ImVec2(100, 100))) {}
		CreateItemDrag(EditorObjectType::GAMEOBJECT);
	}
	ImGui::End();

	if (m_openedInspector) {
		ImGui::SetNextWindowDockID(m_dockspaceRight, ImGuiCond_Always);
		GetInspector()->OnImGui();
	}

	GetAssetSelect()->OnImGui();
}

void EditorWindow::CreateItemDrag(EditorObjectType type) {
	if (ImGui::IsItemActive()) {
		m_currentlyDraggedEditorObjectType = type;
		InstantiateDragging(false);
		ImGuiIO io = ImGui::GetIO();
		// Draw a line between the button and the mouse cursor
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->PushClipRectFullScreen();
		draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
		draw_list->PopClipRect();
	}
}

void EditorWindow::CreateSceneOverview(ImGuiWindowFlags flags) {
	ImGui::SetNextWindowDockID(m_dockspaceLeft, ImGuiCond_Always);
	ImGui::Begin("Layer Manager", nullptr, NULL);

	for (int i = 0; i < m_layers.size(); i++) {
		m_layers[i]->OnImGui();
	}
	ImGui::End();
}

void EditorWindow::DisplayObject(GameObject* obj) {
	String title = Format("%s %s", ICON_FA_BOX, obj->m_name.c_str());

	bool selected = (GetEditorScene()->GetHierarchy().GetSelected() == obj);
}

void EditorWindow::MoveToFolder(HierarchyObject* folder, GameObject* movingChild) {
	return;

	if (movingChild == nullptr) {
		movingChild = m_movingChild;
	}

	HierarchyObject* oldFolder = FindFolderOfObject(movingChild);
	//TODO: Prevents error that needs cleaning up!
	if (oldFolder == NULL)
		return;

	//oldFolder->RemoveChild(m_movingChild);
	//folder->AddChild(m_movingChild);
	m_settingNewFolder = false;
}

HierarchyObject* EditorWindow::FindFolderOfObject(GameObject* obj) {
	for each (HierarchyObject * folder in m_layers) {
		if (folder->ContainsChild(obj)) {
			return folder;
		}
	}

	return NULL;
}

void EditorWindow::ToggleSettingNewParent(GameObject* obj = NULL) {
	if (obj != NULL)
		m_movingChild = obj;

	m_settingNewFolder = !m_settingNewFolder;
}
	
void EditorWindow::AddFolder() {

}

void EditorWindow::CreateViewport() {
	//if (GetStateManager()->GetState() == States::LOADING) return;
	ImGuiViewport* viewport = ImGui::GetWindowViewport();
	ImGuiContext& g = *GImGui;

	ImVec2    actualWindowSize = ImGui::GetContentRegionAvail();
	ImVec2    actualWindowPosition = ImGui::GetCursorScreenPos();
	actualWindowPosition.x -= m_mainWindowPos.x;
	actualWindowPosition.y -= m_mainWindowPos.y;

	m_viewport = Vector4(actualWindowPosition.x, actualWindowPosition.y, actualWindowSize.x, actualWindowSize.y);
	m_mouseInViewport = ImGui::IsWindowHovered(ImGuiHoveredFlags_RectOnly);
	ImGui::Image(GetApp()->GetPipeline()->GetFinalTexture()->GetImGuiHandle(), actualWindowSize, { 0, 1 }, { 1, 0 });
}

void EditorWindow::InstantiateDragging(bool externalSource) {
	m_dragSourceExternal = externalSource;
	m_draggingItem = true;
}

void EditorWindow::SetupEditorStyle(bool bStyleDark, float alpha) {
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