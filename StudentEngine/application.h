#pragma once

class App {
private:
	AsyncQueue<function<void()>> m_queue;
	void HandleQueue();

	bool m_initialized = false;
	Window* m_window = nullptr;
	GLCallbackManager* m_glCallbackManager;
	RenderingPipeline* m_pipeline;
	AssetManager* m_assetManager;
	Timer m_timer;

	bool m_running = true;
	bool m_ImGuiOpen = true;

	uint64 m_frameCount = 0;
	float m_lastFrameTime = 0;
	int m_fps = 0;

public:
	static App* g_app;

	App() {}
	~App();
	
	AssetRef<Window> GetWindow() { return m_window; }
	void OnWindowClose();
	void OnResize(int width, int height);

	void Initialize();
	void Run();
	void Update(TimeStep time);
	void Draw();

	void QueueTask(function<void()> task) {
		m_queue.Add(task);
	}

	template<typename T>
	inline T GetWidth() { return m_window->GetWidth<T>(); }

	template<typename T>
	inline T GetHeight() { return m_window->GetHeight<T>(); }

	inline RenderingPipeline* GetPipeline() {
		return m_pipeline;
	}

	inline AssetManager* GetAssetManager() {
		return m_assetManager;
	}

	inline GLCallbackManager* GetGLCallbackManager() {
		return m_glCallbackManager;
	}
};

inline App* GetApp() { return App::g_app; }
inline Camera* GetCamera() { return GetApp()->GetPipeline()->GetCamera(); }
inline AssetManager* GetAssetManager() { return GetApp()->GetAssetManager(); }
inline GLCallbackManager* GetGLCallbackManager() { return GetApp()->GetGLCallbackManager(); }
