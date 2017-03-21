#ifndef ASSET_H
#define ASSET_H

#include <fstream>
#include <string>
#include "PhysFS.h"

#define TG_ASSET_CTOR(type) tg##type(std::string const& path) : tgRawAsset(path)

class tgAssets;
enum tgAssetType {
	tgTEXTURE_ASSET = 0,
	tgTEXT_ASSET
};

class tgRawAsset {
	friend class tgAssets;
public:
	
	tgRawAsset(std::string const& path) : m_path(path), m_loaded(false) {}

	virtual tgAssetType getType() const = 0;
	virtual void* load(PhysFS::ifstream &stream) = 0;

	bool isLoaded() const { return m_loaded; }
	std::string getPath() const { return m_path; }

protected:
	std::string m_path;
	bool m_loaded;
};

class tgAsset {
public:
	void* data;
	tgAssetType type;
};

#endif
