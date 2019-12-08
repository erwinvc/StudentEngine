#include "stdafx.h"

void AssetWatcher::Initialize() {
	if (m_initialized) return;
	m_thread = GetThreadManager()->RegisterThread("AssetWatcher", [] {GetInstance()->Watch(); });
	LOG("[~yAssets~x] Initialized Asset Watcher");
	AddDirectory("res/shaders", false);
	AddDirectory("res/Assets", false);
	AddChangeHandler(WatchType::CHANGED, "vert", [=](const Tracker& file) { GetShaderManager()->ReloadShaderByFileName(file); });
	AddChangeHandler(WatchType::CHANGED, "frag", [](const Tracker& file) { GetShaderManager()->ReloadShaderByFileName(file); });
	AddChangeHandler(WatchType::CHANGED, "tesc", [](const Tracker& file) { GetShaderManager()->ReloadShaderByFileName(file); });
	AddChangeHandler(WatchType::CHANGED, "tese", [](const Tracker& file) { GetShaderManager()->ReloadShaderByFileName(file); });
	AddChangeHandler(WatchType::ADDED, "png", [](const Tracker& file) {
		String fullName = file.m_path.GetFullPath();
		GetAssetManager()->AddToLoadQueue(new TextureLoadJob(fullName, fullName));
		GetEditorAssetManager()->AddAsset(file.m_path, GetAssetManager()->Get<StreamedTexture>(fullName));
	});
	//AddChangeHandler(WatchType::REMOVED, "", [](const Tracker& file) {
	//	LOG("Folder removed");
	//});
	//AddChangeHandler(WatchType::ADDED, "", [](const Tracker& file) {
	//	LOG("Folder added");
	//});
	AddChangeHandler(WatchType::REMOVED, "png", [](const Tracker& file) {
		GetAssetManager()->Remove(file.m_path.GetFullPath());
		GetEditorAssetManager()->RemoveAsset(file.m_path.GetFullPath());
	});
	AddChangeHandler(WatchType::CHANGED, "png", [](const Tracker& file) {

	});
	m_initialized = true;
}

void AssetWatcher::AddDirectory(const String& dir, bool watchSubTree) {
	if (FileSystem::DoesFileExist(dir)) {
		if (!Utils::VectorContains(m_dirs, dir)) {
			for (auto& file : filesystem::recursive_directory_iterator(dir)) {
				m_paths[file.path().string()] = Tracker(Path(file.path().string().c_str()), filesystem::last_write_time(file));
			}
			m_dirs.push_back(dir);
			LOG("[~yAssets~x] Added ~1%s~x to Asset Watcher", dir.c_str());
			return;
		}
	} else LOG("[~yAssets~x] Failed to create Asset Watcher for ~x%s", dir.c_str());
}

void AssetWatcher::AddChangeHandler(WatchType type, const String& extention, function<void(const Tracker&)> handler) {
	map<String, function<void(const Tracker&)>>* handlers = nullptr;
	switch (type) {
		case WatchType::ADDED: handlers = &m_addedHandlers; break;
		case WatchType::REMOVED: handlers = &m_removedHandlers; break;
		case WatchType::CHANGED: handlers = &m_changedHandlers; break;
	}
	if (handlers->find(extention) == handlers->end()) {
		handlers->emplace(extention, handler);
	} else LOG_ERROR("[~yAssets~x] Asset Watcher already contains handler for ~1%s", extention.c_str());
}

void AssetWatcher::HandleQueue() {
	QueueEntry entry;
	while (m_queue.Size() > 0) {
		if (m_queue.TryToGet(entry)) {
			entry.m_function(entry.m_file);
		}
	}
}