#ifndef ASSET_H
#define ASSET_H

#include <fstream>
#include <string>
#include "PhysFS.h"

#define TG_ASSET_CTOR(type) tg##type(std::string const& path) : tgRawAsset(path)

class tgAssets;
class tgRawAsset {
	friend class tgAssets;
public:
	
	tgRawAsset(std::string const& path) : m_path(path), m_loaded(false) {}

	virtual void* load(PhysFS::ifstream &stream) = 0;

	bool isLoaded() const { return m_loaded; }
	std::string getPath() const { return m_path; }

protected:
	std::string m_path;
	bool m_loaded;
};

#endif
