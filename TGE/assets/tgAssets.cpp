#include "tgAssets.h"
#include "PhysFS.h"

#include <sstream>

#include "../core/tgUtil.h"
#include "../core/tgLog.h"

tgRawAssetQueue tgAssets::m_assetQueue;
tgAssetQueue tgAssets::m_assets;

void tgAssets::create() {
	if (!PhysFS::isInit()) {
		PhysFS::init(nullptr);
	}
}

void tgAssets::destroy() {
	if (PhysFS::isInit()) {
		for (auto &kv : m_assets) {
			tgDelete(kv.second);
		}
		m_assets.clear();

		PhysFS::deinit();
	}
}

void tgAssets::addSource(std::string const& path) {
	PhysFS::addToSearchPath(resolvePath(path));
}

void tgAssets::removeSource(std::string const& path) {
	PhysFS::removeFromSearchPath(resolvePath(path));
}

void tgAssets::load() {
	tgLog::log("Asset Manager Is Loading...");
	for (auto &kv : m_assetQueue) {
		PhysFS::ifstream stream(kv.second->getPath());
		void *asset = kv.second->load(stream);
		m_assets[kv.first] = asset;
		tgLog::log("\tLoaded: ", kv.first);
	}
	m_assetQueue.clear();
	tgLog::log("Asset Manager Is Finished.");
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
