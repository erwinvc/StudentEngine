#pragma once
class AssetManager {
private:
	bool m_initialized = false;
	static int m_activeJobs;
	map<String, AssetBase*> m_assets;
	Texture* m_nullTexture;
	//AssetWatcher* m_assetWatcher;
	AssetRef<Thread> m_loadingThread;
	AsyncQueue<AssetLoadJob*> m_processAssetQueue;
	AsyncQueue<AssetLoadJob*> m_loadAssetQueue;

	template <class T>
	void AddToProcessQueue(T* assetLoadJob) {
		m_processAssetQueue.Add(assetLoadJob);
	}

public:
	AssetManager() {
		if (m_initialized) return;
		m_nullTexture = new Texture(1, 1, Color::White().ToColor8(), TextureParameters(RGBA, RGBA, NEAREST, REPEAT));
		m_initialized = true;
		m_activeJobs = 0;
		//m_assetWatcher = new AssetWatcher();
	}
	~AssetManager() {
		//delete m_assetWatcher;
		delete m_nullTexture;
		for (auto const& [key, val] : m_assets) {
			delete val;
		}
	}

	void Initialize() {
		//m_assetWatcher->Initialize();
	}

	// Execute processing jobs on the main thread.
	void Update() {
		//m_assetWatcher->HandleQueue();
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
				m_assets[name] = new StreamedTexture(m_nullTexture, false);
				return (T*)m_assets[name];
			}
			LOG_WARN("[~yAssets~x] asset ~1%s~x of type ~1%s~x not found", name.c_str(), typeid(T).name());
		}
		return asset;
	}

	template<typename T>
	vector<AssetBase*> GetAllOfType() {
		vector<AssetBase*> results;
		map<String, AssetBase*>::iterator mapIterator;
		for (mapIterator = m_assets.begin(); mapIterator != m_assets.end(); mapIterator++) {
			if (dynamic_cast<T*>(mapIterator->second)) {
				results.push_back(mapIterator->second);
			}
		}
		return results;
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

	//AssetWatcher* GetAssetWatcher() { return m_assetWatcher; }
};