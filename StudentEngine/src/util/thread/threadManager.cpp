#include "stdafx.h"

ThreadManager g_threadManager;

Thread* ThreadManager::RegisterThread(String name, void(*func)()) {
	Thread* c_thread = new Thread(name);
	uint64* args = new uint64[3]{ 0, (uint64)c_thread, (uint64)func };
	c_thread->m_args = args;
	c_thread->m_handle = CreateThread(0, 0, [](void* lpFiberParameter) -> DWORD {
		uint64* arguments = (uint64*)lpFiberParameter;
		Thread* thread = (Thread*)arguments[1];
		//try {
			while (!thread->m_shutDown) {
				((void(*)(void)) arguments[2])();
			}
			LOG("[~rThreads~x] Shut down ~1%s~x thread", thread->GetName().c_str());
			thread->m_finished = true;

		//} catch (...) { LOG_ERROR("[~rThreads~x] caught exception in ~1%s ~rthread", thread->m_name.c_str()); }
		return 0;
	}, args, 0, 0);

	unique_lock<mutex> lock(m_lock);
	m_threads.push_back(c_thread);

	LOG("[~rThreads~x] Registered ~1%s ~xthread ", name.c_str());
	return c_thread;
}

bool ThreadManager::IsShutdown() {
	bool running = false;
	for (auto& thread : m_threads) {
		if (!thread->m_finished) {
			running = true;
		}
	}
	return !running;
}

void ThreadManager::Cleanup() {
	GetThreadPool()->Shutdown();
	for (auto& thread : m_threads) {
		thread->Shutdown();
	}
	while (!IsShutdown()) {
		Sleep(100);
	}

	for (auto& thread : m_threads) {
		delete[] thread->m_args;
		delete thread;
	}
	m_threads.clear();
	//LOG("[~rThreads~x] Thread pool terminated");
}

ThreadManager* GetThreadManager() { return &g_threadManager; }