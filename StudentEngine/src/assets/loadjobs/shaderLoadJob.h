#pragma once

class ShaderLoadJob : public AssetLoadJob {
private:
	String m_file;
	bool m_hasGeometry;
	bool m_hasTessellation;

public:
	ShaderLoadJob(const String& name, const String& file, bool hasGeometry = false, bool hasTessellation = false) : AssetLoadJob(name), m_file(file), m_hasGeometry(hasGeometry), m_hasTessellation(hasTessellation) {}

	bool loadAsset(bool addToProcessQueue = true) override { return true; };
	void processAsset(map<String, AssetBase*>& assets) override;
};