#include "stdafx.h"

Vector2I resizeBuffer(-1, -1);

static void ErrorCallback(int error, const char* description) {
	LOG_ERROR("[GLFW] %s", description);
}

void App::OnResize(int width, int height) {
	if (!m_initialized) return;
	resizeBuffer = Vector2I(width, height);
}

void App::OnWindowClose() {
	m_running = false;
}

void App::Initialize() {
	glfwSetErrorCallback(ErrorCallback);
	
	if (!glfwInit()) LOG_ERROR("[GLFW] GLFW failed to initialize");

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	m_window = new Window("StudentEngine", 1920, 1080);

	m_window->MakeContextCurrent();
	m_window->ClearColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	m_window->SetVSync(true);
	
	if (glewInit() != GLEW_OK) LOG_ERROR("[GLEW] failed to initialize");

	GetGLCallbackManager()->AddOnResizeCallback(this, &App::OnResize);
	GetGLCallbackManager()->AddOnCloseCallback(this, &App::OnWindowClose);

	LOG("[~cGPU~x] %-26s %s", "GPU manufacturer~1", glGetString(GL_VENDOR));
	LOG("[~cGPU~x] %-26s %s", "GPU~1", glGetString(GL_RENDERER));
	LOG("[~cGPU~x] %-26s %s", "OpenGL version~1", glGetString(GL_VERSION));

	m_window->SetIcon(Icon("icon32"));

	GetAssetManager()->Initialize();
	GetStateManager()->Initialize();

	m_pipeline = new RenderingPipeline();
	m_pipeline->Initialize();

	GetGLFiberManager()->Initialize();
	GetGLFiberManager()->AddFiber("Main", [] {GetApp()->Run(); });
	GetGLFiberManager()->AddFiber("AssetManager", [] {GetAssetManager()->Update(); });
	GetGLFiberManager()->AddFiber("Tween", [] {});
	
	m_window->Show();
	m_initialized = true;

	while (m_running) {
		GetGLFiberManager()->Tick();
	}
}

void App::HandleQueue() {
	function<void()> task;
	if (m_queue.TryToGet(task)) {
		task();
	}
}

void App::Run() {
	m_timer = Timer();
	float totalTime = 0;
	float timer = m_timer.Get();
	float updateTimer = m_timer.Get();
	float updateTick = 1000.0f / 60.0f;
	double delta = 0;
	int frames = 0, updates = 0;
	while (m_running) {
		m_window->Clear();
		float time = m_timer.Get();
		if (time - updateTimer > updateTick) {
			totalTime += time - m_lastFrameTime;
			Update(TimeStep(time - m_lastFrameTime, totalTime, m_frameCount));
			m_lastFrameTime = time;
			updates++;
			updateTimer += updateTick;
			m_frameCount++;
		}
		delta += (time - updateTimer) / updateTick;
		Draw();
		frames++;
		if (glfwGetTime() - timer > 1.0) {
			m_window->SetTitle(Format_t("StudentEngine | UPS: %d FPS: %d", updates, frames));
			m_fps = frames;
			timer++;
			updates = frames = 0;
		}
		GetGLFiberManager()->GoToMainFiber();
	}
}

void App::Update(TimeStep time) {
	GetMouse()->Update();
	GetStateManager()->Update(time);
	GetTweenManager()->Update(time);

	m_pipeline->Update(time);

	GetAssetManager()->Update();
}

void App::Draw() {
	m_pipeline->m_camera->UpdateViewMatrix();
	m_pipeline->Begin();
	GetStateManager()->Draw(m_pipeline);
	m_pipeline->End();
	GetImGuiManager()->Begin();

	if (ImGui::Begin("Dev###Window2", &m_ImGuiOpen, ImVec2(100, 200), ImGuiWindowFlags_NoDocking)) {
		if (ImGui::BeginTabBar("Tab###1", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)) {
			m_pipeline->OnImGui();
			GetFrameBufferManager()->OnImGui();
			GetShaderManager()->OnImGui();
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	GetStateManager()->OnImGui();
	GetImGuiManager()->End();

	GetStateManager()->PostDraw(m_pipeline);

	if (resizeBuffer.x != -1) {
		uint width = resizeBuffer.x;
		uint height = resizeBuffer.y;
		glViewport(0, 0, width, height);
		m_window->SetWidth(width);
		m_window->SetHeight(height);
		GetFrameBufferManager()->OnResize(width, height);
		//GetStateManager()->OnResize(width, height);
		resizeBuffer = Vector2I(-1, -1);
	}

	GetAssetWatcher()->HandleQueue();
	HandleQueue();

	//GetAssetWatcher()->HandleQueue();

	m_window->SwapBuffers();
	m_window->PollEvents();
}

void App::Cleanup() {
	Undo::Cleanup();
	GetThreadManager()->Cleanup();
	delete m_pipeline;
	delete m_window;
}