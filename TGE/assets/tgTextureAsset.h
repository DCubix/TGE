#ifndef TEXTURE_ASSET_H
#define TEXTURE_ASSET_H

#include "tgAsset.h"

class tgTextureAsset : public tgRawAsset {
public:
	TG_ASSET_CTOR(TextureAsset) {}

	tgAssetType getType() const override { return tgAssetType::tgTEXTURE_ASSET;  }
	void* load(PhysFS::ifstream &stream) override;
};

#endif
