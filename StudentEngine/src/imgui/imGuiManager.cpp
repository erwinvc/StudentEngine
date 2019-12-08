#include "stdafx.h"

ImFont* ImGuiManager::g_bigFont;
ImFont* ImGuiManager::g_smallFont;
void ImGuiManager::Initialize(Window* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.IniFilename = NULL;
	//io.ConfigDockingWithShift = true;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), false);
	ImGui_ImplOpenGL3_Init("#version 410");

	GetGLCallbackManager()->AddOnMouseButtonCallback([](int button, int action, int mods) {
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLFW_PRESS) io.MouseDown[button] = true;
		else if (action == GLFW_RELEASE) io.MouseDown[button] = false;
	});

	GetGLCallbackManager()->AddOnMousePosCallback([](int x, int y) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2((float)x, (float)y);
	});

	GetGLCallbackManager()->AddOnScrollCallback([](double x, double y) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += (float)y;
		io.MouseWheelH += (float)x;
	});

	GetGLCallbackManager()->AddOnKeyCallback([](int key, int scancode, int action, int mods) {
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLFW_PRESS) io.KeysDown[key] = true;
		else if (action == GLFW_RELEASE) io.KeysDown[key] = false;
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	});

	GetGLCallbackManager()->AddOnResizeCallback([](int width, int height) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)width, (float)height);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	});

	GetGLCallbackManager()->AddOnKeyTypedEvent([](uint c) {
		ImGuiIO& io = ImGui::GetIO();
		if (c > 0 && c < 0x10000)
			io.AddInputCharacter((unsigned short)c);
	});

	g_smallFont = io.Fonts->AddFontFromFileTTF("res/fonts/Consolas.ttf", 10.0f, NULL, io.Fonts->GetGlyphRangesDefault());
	g_bigFont = io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/Consolas.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesDefault());

	// Get the icons from both FA regular & solid (each having their own range of different icons)
	// And add them to the already existing font
	ImFontConfig config;
	config.MergeMode = true;
	const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromFileTTF("res/fonts/fa-regular-400.ttf", 15.0f, &config, icon_ranges);
	io.Fonts->AddFontFromFileTTF("res/fonts/fa-solid-900.ttf", 15.0f, &config, icon_ranges);
	io.Fonts->Build();
}

void ImGuiManager::Begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void ImGuiManager::End() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	GLFWwindow* backup = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(backup);
}

bool ImGui::NamedButton(Texture* texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col, const char* label, bool double_click) {
	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	// Default to using texture ID as ID. User can still push string/integer prefixes.
	// We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
	//PushID((void*)(intptr_t)user_texture_id);
	const ImGuiID id = window->GetID(label);
	ImVec2 label_size = CalcTextSize(label, NULL, true);
	bool popFont = false;
	float y = 0;
	if (label_size.x > size.x) {
		ImGui::PushFont(ImGuiManager::g_smallFont);
		label_size.x = CalcTextSize(label, NULL, true).x;
		y += ImGui::GetFontSize() * 0.75f;
		popFont = true;
	}

	//PopID();

	// add 3 font size to y for button rectangle height;
	ImVec2 button_size = size;
	button_size.y += label_size.y * 2;

	//Get aspect
	ImVec2 aspect(size.y / size.x, 1.0f);
	float w = texture->GetWidth();
	float h = texture->GetHeight();
	if (w < h) {
		aspect.x *= w / h;
	} else if (h < w) {
		aspect.y *= h / w;
	}

	ImVec2 aspectSize = aspect * size;

	const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + button_size + padding * 2);
	ImRect image_bb(window->DC.CursorPos + padding + size / 2 - aspectSize / 2, window->DC.CursorPos + padding + size / 2 + aspectSize / 2);
	y += (window->DC.CursorPos + padding + size).y;
	ItemSize(bb);
	if (!ItemAdd(bb, id)) {
		if (popFont) ImGui::PopFont();
		return false;
	}

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	// Render
	//const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	const ImU32 col = GetColorU32(held ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	if (bg_col.w > 0.0f)
		window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

	void* handle = (void*)texture->GetHandle();

	ImVec2 aspect2 = { image_bb.Max.x * aspect.x, image_bb.Max.y * aspect.y };

	window->DrawList->AddImage(handle, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

	ImRect offset = bb;
	offset.Min.y = y + padding.y;


	//RenderTextWrapped(offset.Min + style.FramePadding, label, NULL, label_size.x - padding.x);
	offset.Max.x -= style.FramePadding.x;
	RenderTextClipped(offset.Min + style.FramePadding, offset.Max, label, NULL, &label_size, style.ButtonTextAlign, &offset);

	if (popFont) ImGui::PopFont();

	return pressed;
}