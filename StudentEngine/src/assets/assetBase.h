#pragma once

class AssetBase {
private:
	Path m_filePath;
public:
	AssetBase() {};
	AssetBase(const Path& filePath) : m_filePath(filePath) {};
	virtual ~AssetBase() {}
	Path& GetFilePath() {
		return m_filePath;
	}
	//String m_fullName;
	//String m_displayName;
	//
	//AssetBase(const String& fullName, const String& displayName) : m_fullName(fullName), m_displayName(displayName) {}
};