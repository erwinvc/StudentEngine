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

	virtual void loadAsset(bool addToProcessQueue = true) {

	}

	virtual void processAsset(map<String, AssetBase*>& assets) {

	}

	const String& GetID() { return m_id; }
};

