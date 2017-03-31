#include "tgAssets.h"

#include <sstream>
#include <regex>
#include <physfs.h>

#include "../core/tgUtil.h"
#include "../core/tgLog.h"

#include "../core/tgImageData.h"
#include "../audio/tgSampleData.h"

tgAssetList<tgTexture*> tgAssets::m_textures;
tgAssetList<tgFont*> tgAssets::m_fonts;
tgAssetList<tgAudioBuffer*> tgAssets::m_sounds;
tgAssetList<std::string> tgAssets::m_texts;

void tgAssets::create() {
	if (!PHYSFS_isInit()) {
		PHYSFS_init(NULL);
	}
}

void tgAssets::destroy() {
	if (PHYSFS_isInit()) {
		for (auto &kv : m_textures) {
			tgDelete(kv.second);
		}
		for (auto &kv : m_fonts) {
			tgDelete(kv.second);
		}
		for (auto &kv : m_sounds) {
			tgDelete(kv.second);
		}
		m_textures.clear();
		m_fonts.clear();
		m_sounds.clear();
		m_texts.clear();

		PHYSFS_deinit();
	}
}

void tgAssets::addSource(std::string const& path) {
	PHYSFS_addToSearchPath(resolvePath(path).c_str(), 1);
}

tgTexture* tgAssets::getTexture(std::string const& file) {
	auto pos = m_textures.find(file);
	if (pos == m_textures.end()) {
		tgAssetData *data = loadRawAsset(file);
		if (data) {
			m_textures[file] = new tgTexture(tgImageData(data), tgTexture::tgTEXTURE_RGBA);
			return m_textures[file];
		}
	} else {
		return pos->second;
	}
	return nullptr;
}

tgFont* tgAssets::getFont(std::string const& file) {
	auto pos = m_fonts.find(file);
	if (pos == m_fonts.end()) {
		tgAssetData *data = loadRawAsset(file);
		if (data) {
			m_fonts[file] = new tgFont(data);
			return m_fonts[file];
		}
	} else {
		return pos->second;
	}
	return nullptr;
}

tgAudioBuffer* tgAssets::getSound(std::string const& file) {
	auto pos = m_sounds.find(file);
	if (pos == m_sounds.end()) {
		tgAssetData *data = loadRawAsset(file);
		if (data) {
			m_sounds[file] = new tgAudioBuffer(tgSampleData(data));
			return m_sounds[file];
		}
	} else {
		return pos->second;
	}
	return nullptr;
}

std::string tgAssets::getText(std::string const& file) {
	auto pos = m_texts.find(file);
	if (pos == m_texts.end()) {
		tgAssetData *data = loadRawAsset(file);
		if (data) {
			m_texts[file] = std::string(reinterpret_cast<const char*>(data->data), data->size);
			return m_texts[file];
		}
	} else {
		return pos->second;
	}
	return std::string();
}

std::string tgAssets::resolvePath(std::string const& path) {
	std::string rpath;
	if (path == ".") {
		rpath = std::string(PHYSFS_getBaseDir());
	} else {
		std::stringstream stm;
		stm << PHYSFS_getBaseDir() << "/" << path;
		rpath = stm.str();
	}
	return rpath;
}

tgAssetData* tgAssets::loadRawAsset(std::string const& path) {
	const char* cpath = path.c_str();
	if (PHYSFS_exists(cpath)) {
		PHYSFS_file *f = PHYSFS_openRead(cpath);
		PHYSFS_sint64 len = PHYSFS_fileLength(f);
		byte *buf = new byte[len];
		PHYSFS_sint64 lengthRead = PHYSFS_read(f, buf, 1, len);
		PHYSFS_close(f);

		tgAssetData *data = new tgAssetData();
		data->data = buf;
		data->size = uint(lengthRead);
		return data;
	}
	return nullptr;
}