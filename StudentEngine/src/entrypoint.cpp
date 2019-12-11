#include "stdafx.h"

App* App::g_app;

int main() {
	srand((uint)time(NULL));

	Logger::Initialize();
	App::g_app = new App();
	App::g_app->Initialize();
	delete App::g_app;
	
	if (Memory::CheckAllocations() != 0)__debugbreak();
	Logger::Cleanup();
	
	//_CrtDumpMemoryLeaks();
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return main();
}