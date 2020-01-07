#pragma once

class AssetBase {
protected:
	String m_name;
	Path m_filePath;
public:
	AssetBase(const String& name) : m_name(name) {};
	AssetBase(const String& name, const Path& filePath) : m_name(name), m_filePath(filePath) {};
	virtual ~AssetBase() {}

	Path& GetFilePath() {
		return m_filePath;
	}

	String& GetFullFileName() {
		return m_filePath.GetFileName() + "." + m_filePath.GetExtention();
	}

	virtual AssetBase* GetTexture() {
		return NULL;
	}

	String& GetName() {
		return m_name;
	}

	//String m_fullName;
	//String m_displayName;
	//
	//AssetBase(const String& fullName, const String& displayName) : m_fullName(fullName), m_displayName(displayName) {}
};