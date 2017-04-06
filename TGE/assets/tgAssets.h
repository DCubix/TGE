#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <unordered_map>

#include "../graphics/tgTexture.h"
#include "../core/tgFont.h"
#include "../audio/tgAudioBuffer.h"

#include "../core/tgImageData.h"
#include "../audio/tgSampleData.h"

#include "tgAssetData.h"

template <typename T>
using tgAssetList = std::unordered_map<std::string, T>;

class tgAssets {
public:
	static void create();
	static void destroy();

	static void addSource(std::string const& path);

	static tgTexture* getTexture(std::string const& file);
	static tgFont* getFont(std::string const& file);
	static tgAudioBuffer* getSound(std::string const& file);
	static std::string getText(std::string const& file);

private:
	static std::string resolvePath(std::string const& path);
	static tgAssetData* loadRawAsset(std::string const& path);
	
	static tgAssetList<tgTexture*> m_textures;
	static tgAssetList<tgFont*> m_fonts;
	static tgAssetList<tgAudioBuffer*> m_sounds;
	static tgAssetList<std::string> m_texts;
};

#endif
