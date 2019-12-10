#pragma once
class AssetManager : public Singleton<AssetManager> {
protected:
	AssetManager() {}
	~AssetManager() { delete m_nullTexture; }

	friend Singleton;
private:
	bool m_initialized;
	static int m_activeJobs;
	map<String, AssetBase*> m_assets;
	Texture* m_nullTexture;
	AssetRef<Thread> m_loadingThread;
	AsyncQueue<AssetLoadJob*> m_processAssetQueue;
	AsyncQueue<AssetLoadJob*> m_loadAssetQueue;

	// Excecute loading jobs on seperate thread.
	//void ExecuteLoadJobs() {
	//	if (m_loadAssetQueue.Size() != 0) {
	//		AssetLoadJob* currentLoadJob;
	//
	//		if (m_loadAssetQueue.TryToGet(currentLoadJob)) {
	//			if (currentLoadJob->loadAsset()) {
	//				AddToProcessQueue(currentLoadJob);
	//			} else {
	//				delete currentLoadJob;
	//				m_activeJobs--;
	//			}
	//		}
	//	}
	//}

	template <class T>
	void AddToProcessQueue(T* assetLoadJob) {
		m_processAssetQueue.Add(assetLoadJob);
	}

public:
	void Initialize() {
		if (m_initialized) return;
		//m_loadingThread = GetThreadManager()->RegisterThread("AssetManager LoadJobs", []() {GetInstance()->ExecuteLoadJobs(); });
		m_nullTexture = new Texture(1, 1, Color::White().ToColor8(), TextureParameters(RGBA, RGBA, NEAREST, REPEAT));
		m_initialized = true;
		m_activeJobs = 0;
	}

	// Execute processing jobs on the main thread.
	void Update() {
		if (m_processAssetQueue.Size() != 0) {
			AssetLoadJob* currentLoadJob;
			if (m_processAssetQueue.TryToGet(currentLoadJob)) {
				currentLoadJob->ProcessAsset(m_assets);
				m_activeJobs--;
				delete currentLoadJob;
			}
		}
	}

	template <class T>
	void AddToLoadQueue(T* assetLoadJob) {
		GetThreadPool()->DoJob([=] {
			if (assetLoadJob->LoadAsset()) {
				AddToProcessQueue(assetLoadJob);
			} else {
				delete assetLoadJob;
				m_activeJobs--;
			}
		});
		m_activeJobs++;
	}

	int GetActiveJobs() { return m_activeJobs; }

	template<typename T>
	T* Get(const String& name) {
		T* asset = (T*)m_assets[name];
		if (!asset) {
			if (typeid(T) == typeid(StreamedTexture)) {
				return (T*)(m_assets[name] = new StreamedTexture(m_nullTexture, false));
			}
			LOG_WARN("[~yAssets~x] asset ~1%s~x of type ~1%s~x not found", name.c_str(), typeid(T).name());
		}
		return asset;
	}

	template<typename T>
	void Add(const String& name, T* asset) {
		if (m_assets[name]) LOG_WARN("[~yAssets~x] asset ~1%s~x of type ~1%s~x already exists", name.c_str(), typeid(T).name());
		else m_assets[name] = asset;
	}

	void Remove(const String& name) {
		if (m_assets[name]) {
			LOG("[~yAssets~x] removed ~1%s~x", name.c_str());
			delete m_assets[name];
			m_assets.erase(name);
		} else LOG_WARN("[~yAssets~x] failed to remove %s", name.c_str());
	}

	template<typename T>
	AssetRef<T> ForceLoadAsset(AssetLoadJob* loader) {
		loader->LoadAsset(false);
		loader->ProcessAsset(m_assets);
		T* asset = (T*)m_assets[loader->GetID()];
		delete loader;
		return asset;
	}

	Texture* GetNullTexture() {
		return m_nullTexture;
	}
};

static AssetManager* GetAssetManager() {
	return AssetManager::GetInstance();
}