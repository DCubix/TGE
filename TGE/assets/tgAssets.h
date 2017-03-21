#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <unordered_map>
#include <algorithm>

#include "tgAsset.h"

using tgRawAssetQueue = std::unordered_map<std::string, tgRawAsset*>;
using tgAssetQueue = std::unordered_map<std::string, tgAsset*>;
class tgAssets {
public:
	static void create();
	static void destroy();

	static void addSource(std::string const& path);
	static void removeSource(std::string const& path);

	template <typename T>
	static void add(std::string const& path) {
		std::string name = path.substr(path.find_last_of("/\\") + 1);
		auto pos = m_assetQueue.find(name);
		if (pos == m_assetQueue.end()) {
			m_assetQueue[name] = new T(path);
		}
	}

	template <typename T>
	static T* get(std::string const& name) {
		auto pos = m_assets.find(name);
		if (pos != m_assets.end()) {
			return static_cast<T*>(m_assets[name]->data);
		}
		return nullptr;
	}

	static void load();

private:
	static std::string resolvePath(std::string const& path);
	
	static tgRawAssetQueue m_assetQueue;	///< Assets that are waiting to be loaded
	static tgAssetQueue m_assets;			///< Assets that have been loaded
};

#endif
