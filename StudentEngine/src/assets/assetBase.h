#pragma once

class AssetBase {
protected:
	Path m_filePath;
public:
	AssetBase() {};
	AssetBase(const Path& filePath) : m_filePath(filePath) {};
	virtual ~AssetBase() {}

	Path& GetFilePath() {
		return m_filePath;
	}

	String GetFullFileName() {
		return m_filePath.GetFileName() + "." + m_filePath.GetExtention();
	}

	virtual AssetBase* GetTexture() {
		return NULL;
	}

	//String m_fullName;
	//String m_displayName;
	//
	//AssetBase(const String& fullName, const String& displayName) : m_fullName(fullName), m_displayName(displayName) {}
};