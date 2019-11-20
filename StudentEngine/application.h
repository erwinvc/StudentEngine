#pragma once

class App : public Singleton<App> {
private:
	bool m_initialized = false;
	Window* m_window = nullptr;
	Timer m_timer;

	bool m_running = true;
	bool m_ImGuiOpen = true;

	uint64 m_frameCount = 0;
	float m_lastFrameTime = 0;
	int m_fps = 0;

public:
	void OnWindowClose();
	void OnResize(int width, int height);

	void Initialize();
	void Run();
	void Update(TimeStep time);
	void Render();
	void Cleanup();

protected:
	App() {}
	~App() {}
	friend Singleton;
};

inline App* GetApp() { return App::GetInstance(); }