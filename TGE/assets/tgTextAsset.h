#ifndef TEXT_ASSET_H
#define TEXT_ASSET_H

#include "tgAsset.h"

class tgTextAsset : public tgRawAsset {
public:
	TG_ASSET_CTOR(TextAsset) {}

	void* load(PhysFS::ifstream &stream) override;
};

#endif
