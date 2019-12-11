#pragma once
class AssetLoadJob
{
protected:
	String m_id;
public:
	AssetLoadJob() {
		m_id = "Empty LoadJob";
	}

	AssetLoadJob(const String& id) : m_id(id) {

	}

	virtual ~AssetLoadJob() {
	}

	virtual bool loadAsset(bool addToProcessQueue = true) = 0;

	virtual void processAsset(map<String, AssetBase*>& assets) = 0;

	const String& GetID() { return m_id; }
};