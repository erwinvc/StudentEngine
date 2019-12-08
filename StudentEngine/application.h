#pragma once

class App : public Singleton<App> {
private:
	AsyncQueue<function<void()>> m_queue;
	void HandleQueue();

	bool m_initialized = false;
	Window* m_window = nullptr;
	RenderingPipeline* m_pipeline;
	Timer m_timer;

	bool m_running = true;
	bool m_ImGuiOpen = true;

	uint64 m_frameCount = 0;
	float m_lastFrameTime = 0;
	int m_fps = 0;

public:
	AssetRef<Window> GetWindow() { return m_window; }
	void OnWindowClose();
	void OnResize(int width, int height);

	void Initialize();
	void Run();
	void Update(TimeStep time);
	void Draw();
	void Cleanup();

	void QueueTask(function<void()> task) {
		m_queue.Add(task);
	}

	template<typename T>
	inline T GetWidth() { return m_window->GetWidth<T>(); }

	template<typename T>
	inline T GetHeight() { return m_window->GetHeight<T>(); }

	RenderingPipeline* GetPipeline() {
		return m_pipeline;
	}

protected:
	App() {}
	~App() {}
	friend Singleton;
};

inline App* GetApp() { return App::GetInstance(); }
inline Camera* GetCamera() { return App::GetInstance()->GetPipeline()->GetCamera(); }