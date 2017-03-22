#ifndef FONT_ASSET_H
#define FONT_ASSET_H

#include "tgAsset.h"

class tgFontAsset : public tgRawAsset {
public:
	TG_ASSET_CTOR(FontAsset) {}

	void* load(PhysFS::ifstream &stream) override;
};

#endif
