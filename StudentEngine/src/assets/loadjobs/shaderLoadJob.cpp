#include "stdafx.h"

void ShaderLoadJob::ProcessAsset(map<String, AssetBase*>& assets) {
	GetShaderManager()->Create(m_id, m_file, m_hasGeometry, m_hasTessellation);
}