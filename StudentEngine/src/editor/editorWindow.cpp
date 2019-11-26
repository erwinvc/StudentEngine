#include "stdafx.h"

void editorWindow::Initialize() {}
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
		if (ImGui::BeginMenu("Return to Edit")) {
			inEditorMode = true;
			ImGui::EndMenu();
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
		if (ImGui::BeginMenu("Enter Play Mode")) {
			inEditorMode = false;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	static ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	// Main viewport
	ImGui::SetNextWindowDockID(m_dockspaceCenter, ImGuiCond_Always);
	if (ImGui::Begin("Editor Window###EditorWindow", nullptr, window_flags2)) {
		if (ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)) {

			CreateViewport();
			
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	//ImGui::Begin("Awindow", nullptr, ImVec2(576, 680), ImGuiWindowFlags_NoDocking);
//ImGui::End();
//
	// Hierarchy
	ImGui::SetNextWindowDockID(m_dockspaceLeft, ImGuiCond_Always);
	ImGui::Begin("Hierarchy", nullptr, window_flags2);
	if (ImGui::TreeNode("GameObject")) {
		if (ImGui::TreeNode("Child GameObject")) { ImGui::TreePop(); }
		ImGui::TreePop();
	}
	GetPipeline()->GetSpriteRenderer()->OnImGui();
	ImGui::BulletText("Camera");
	ImGui::BulletText("Player");
	ImGui::BulletText("etc.");
	ImGui::End();

	// Drag 'n Drop
	ImGui::SetNextWindowDockID(m_dockspaceLeftBottom, ImGuiCond_Always);

	if (ImGui::Begin("Items", nullptr, window_flags2)) {
		if (ImGui::Button("Sprite")) {}
		if (ImGui::Button("GameObject")) {}
		if (ImGui::Button("Scripts...")) {}
		if (ImGui::Button("Stuff")) {}
	}
	ImGui::End();

	// Throwing the logger in there for debugging purposes (for now)
	ImGui::SetNextWindowDockID(m_dockspaceBottom, ImGuiCond_Always);
	Logger::OnImGui();

	ImGui::PopStyleVar(3);
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
