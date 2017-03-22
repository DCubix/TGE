#ifndef TEXTURE_ASSET_H
#define TEXTURE_ASSET_H

#include "tgAsset.h"

class tgTextureAsset : public tgRawAsset {
public:
	TG_ASSET_CTOR(TextureAsset) {}

	void* load(PhysFS::ifstream &stream) override;
};

#endif
