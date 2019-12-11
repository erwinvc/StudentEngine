#include "stdafx.h"

void ThreadPool::Initialize(int threads) {
	if (m_initialized) return;
	m_initialized = true;

	for (int i = 0; i < threads; ++i) {
		GetThreadManager()->RegisterThread(Format("Job Thread %d", i), [] {GetThreadPool()->ThreadEntry(); });
	}
	LOG("[~rThreads~x] Initialized Thread Pool");
}

void ThreadPool::DoJob(std::function <void(void)> func) {
	if (!m_initialized) {
		LOG_ERROR("[~rThreads~x] Thread pool has not yet been initialized");
		return;
	}
	m_queue.Add(func);
}

void ThreadPool::ThreadEntry() {
	function <void(void)> job;
	if (m_queue.WaitForGet(job)) {
		job();
	}
}