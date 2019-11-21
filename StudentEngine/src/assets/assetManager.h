#pragma once
class AssetManager : public Singleton<AssetManager>
{
private:
	bool m_initialized;
	map<String, AssetBase*> m_assets;
	AssetRef<Thread> m_loadingThread;
	bool m_loadingThreadActive;
	AsyncQueue<AssetLoadJob*> m_processAssetQueue;
	AsyncQueue<AssetLoadJob*> m_loadAssetQueue;

	void ExecuteLoadJobs() {
		// Excecute loading jobs on seperate thread.
		while (m_loadingThreadActive) {
			if (m_loadAssetQueue.Size() != 0) {
				AssetLoadJob* currentLoadJob;
				if (m_loadAssetQueue.TryToGet(currentLoadJob)) {
					currentLoadJob->loadAsset();
				}
			}
		}
	}
public:
	void Initialize() {
		if (m_initialized) {
			return;
		}
		m_loadingThreadActive = true;
		m_loadingThread = GetThreadManager()->RegisterThread("AssetManager LoadJobs", []() {GetInstance()->ExecuteLoadJobs(); });
		m_initialized = true;
	}

	void Update() {
		// Execute processing jobs on the main thread.
		if (m_processAssetQueue.Size() != 0) {
			AssetLoadJob* currentLoadJob;
			if (m_processAssetQueue.TryToGet(currentLoadJob)) {
				currentLoadJob->processAsset(m_assets);
			}
		}
	}

	template <class T>
	void AddToLoadQueue(T* assetLoadJob) {
		m_loadAssetQueue.Add(assetLoadJob);
	}

	template <class T>
	void AddToProcessQueue(T* assetLoadJob) {
		m_processAssetQueue.Add(assetLoadJob);
	}
};

static AssetManager* GetAssetManager() {
	return AssetManager::GetInstance();
}