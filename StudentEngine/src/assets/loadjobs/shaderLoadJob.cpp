#include "stdafx.h"

void ShaderLoadJob::processAsset(map<String, AssetBase*>& assets) {
	GetShaderManager()->Create(m_id, m_file, m_hasGeometry, m_hasTessellation);
}