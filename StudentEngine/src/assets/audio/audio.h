#pragma once
enum class AudioType {
	MP3
};

class Audio : public AssetBase
{
private:
	Path m_filePath;
};