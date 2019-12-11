#pragma once
#include <filesystem>

enum class WatchType {
	ADDED,
	REMOVED,
	CHANGED
};

struct Tracker {
	Path m_path;
	filesystem::file_time_type m_time;


	Tracker(Path& path, filesystem::file_time_type& time) : m_path(path), m_time(time) {}
	Tracker() : m_path(), m_time(filesystem::file_time_type()) {}
};

class AssetWatcher {
private:
	struct QueueEntry {
		function<void(const Tracker&)> m_function;
		Tracker m_file;
	};

	AsyncQueue<QueueEntry> m_queue;
	unordered_map<String, Tracker> m_paths;
	map<String, function<void(const Tracker&)>> m_removedHandlers;
	map<String, function<void(const Tracker&)>> m_addedHandlers;
	map<String, function<void(const Tracker&)>> m_changedHandlers;
	AssetRef<Thread> m_thread;
	bool m_initialized = false;
	vector<String> m_dirs;
	void Watch() {
		Sleep(500);

		for (auto it = m_paths.begin(); it != m_paths.end();) {
			if (!filesystem::exists(it->first)) {
				HandleChange(it->second, WatchType::REMOVED);
				it = m_paths.erase(it);
			} else it++;
		}

		for (String& dir : m_dirs) {
			for (auto& file : std::filesystem::recursive_directory_iterator(dir)) {
				if (!filesystem::exists(file)) continue;
				auto current_file_last_write_time = std::filesystem::last_write_time(file);

				filesystem::path path = file.path();
				auto it = m_paths.find(path.string());
				if (it == m_paths.end()) {
					Tracker newTracker(Path(path.string()), current_file_last_write_time);
					m_paths[file.path().string()] = newTracker;
					HandleChange(newTracker, WatchType::ADDED);
				} else {
					if (it->second.m_time != current_file_last_write_time) {
						it->second.m_time = current_file_last_write_time;
						HandleChange(it->second, WatchType::CHANGED);
					}
				}
			}
		}
	}

	void HandleChange(const Tracker& tracker, WatchType type) {
		map<String, function<void(const Tracker&)>>* handlers = nullptr;
		switch (type) {
			case WatchType::ADDED: handlers = &m_addedHandlers; break;
			case WatchType::REMOVED: handlers = &m_removedHandlers; break;
			case WatchType::CHANGED: handlers = &m_changedHandlers; break;
		}

		//switch (type) {
		//	case WatchType::ADDED:   LOG("[~yAssets~x] Detected added ~1%s", tracker.m_path.GetFileName().c_str()); break;
		//	case WatchType::REMOVED: LOG("[~yAssets~x] Detected removed ~1%s", tracker.m_path.GetFileName().c_str()); break;
		//	case WatchType::CHANGED: LOG("[~yAssets~x] Detected change in ~1%s", tracker.m_path.GetFileName().c_str()); break;
		//}
		auto it = handlers->find(tracker.m_path.GetExtention());
		if (it != handlers->end()) {
			m_queue.Add({ it->second, tracker });
		}
	}
public:
	void Initialize();
	void AddDirectory(const String& dir, bool watchSubTree);
	void AddChangeHandler(WatchType type, const String& extention, function<void(const Tracker&)> handler);
	void HandleQueue();
};