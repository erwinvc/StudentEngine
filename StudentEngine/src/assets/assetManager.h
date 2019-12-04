#pragma once
class AssetManager : public Singleton<AssetManager> {
protected:
	AssetManager() {}
	~AssetManager() { delete m_nullTexture; }

	friend Singleton;
private:
	bool m_initialized;
	int m_activeJobs;
	map<String, AssetBase*> m_assets;
	Texture* m_nullTexture;
	AssetRef<Thread> m_loadingThread;
	AsyncQueue<AssetLoadJob*> m_processAssetQueue;
	AsyncQueue<AssetLoadJob*> m_loadAssetQueue;

	// Excecute loading jobs on seperate thread.
	void ExecuteLoadJobs() {
		if (m_loadAssetQueue.Size() != 0) {
			AssetLoadJob* currentLoadJob;

			if (m_loadAssetQueue.TryToGet(currentLoadJob)) {
				if (currentLoadJob->loadAsset()) {
					AddToProcessQueue(currentLoadJob);
				} else {
					delete currentLoadJob;
					m_activeJobs--;
				}
			}
		}
	}

	template <class T>
	void AddToProcessQueue(T* assetLoadJob) {
		m_processAssetQueue.Add(assetLoadJob);
	}

public:
	void Initialize() {
		if (m_initialized) return;
		m_loadingThread = GetThreadManager()->RegisterThread("AssetManager LoadJobs", []() {GetInstance()->ExecuteLoadJobs(); });
		m_nullTexture = new Texture(1, 1, Color::White().ToColor8(), TextureParameters(RGBA, RGBA, NEAREST, REPEAT));
		m_initialized = true;
		m_activeJobs = 0;
	}

	// Execute processing jobs on the main thread.
	void Update() {
		if (m_processAssetQueue.Size() != 0) {
			AssetLoadJob* currentLoadJob;
			if (m_processAssetQueue.TryToGet(currentLoadJob)) {
				currentLoadJob->processAsset(m_assets);
				m_activeJobs--;
				delete currentLoadJob;
			}
		}
	}

	template <class T>
	void AddToLoadQueue(T* assetLoadJob) {
		m_loadAssetQueue.Add(assetLoadJob);
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

	template<typename T>
	AssetRef<T> ForceLoadAsset(AssetLoadJob* loader) {
		loader->loadAsset(false);
		loader->processAsset(m_assets);
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