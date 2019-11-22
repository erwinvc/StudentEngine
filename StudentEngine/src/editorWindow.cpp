#include "stdafx.h"

void editorWindow::Initialize() {

}

void editorWindow::OnImGui() {
	CreateDockingSpace();

	CreateEditorWindows();
}

void editorWindow::End() {}

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
	ImGui::SetNextWindowDockID(m_dockspaceCenter, ImGuiCond_Always);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	// Main viewport
	if (ImGui::Begin("Editor Window###Window", nullptr, window_flags2)) {
		if (ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)) {

			//fancy texture render pipeline here
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 parentPos = ImGui::GetCurrentWindowRead()->ParentWindow->Pos;

			//GetPipeline()->OnResize((uint)viewportSize.x, (uint)viewportSize.y);
			if (viewportSize.x > 0 && viewportSize.y > 0)
				GetFrameBufferManager()->OnResize((uint)viewportSize.x, (uint)viewportSize.y);
			//GetCamera()->UpdateProjectionMatrix();

			//Hardcoded 19 because we can't get this value from the parent window with ImGui::GetCurrentWindowRead()->ParentWindow->MenuBarHeight(); 
			GetRenderingPipeline()->m_camera->SetViewport(pos.x - parentPos.x, pos.y - parentPos.y + 19, viewportSize.x, viewportSize.y);
			ImGui::Image((void*)GetRenderingPipeline()->GetFinalTexture()->GetHandle(), viewportSize, { 0, 1 }, { 1, 0 });
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	// Hierarchy
	ImGui::SetNextWindowDockID(m_dockspaceLeft, ImGuiCond_Always);
	ImGui::Begin("Hierarchy", nullptr, window_flags2);
	if (ImGui::TreeNode("GameObject")) {
		if (ImGui::TreeNode("Child GameObject")) { ImGui::TreePop(); }
		ImGui::TreePop();
	}
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
