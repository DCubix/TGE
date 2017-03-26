#ifndef AUDIO_ASSET_H
#define AUDIO_ASSET_H

#include "tgAsset.h"

class tgAudioAsset : public tgRawAsset {
public:
	TG_ASSET_CTOR(AudioAsset) {}

	void* load(PhysFS::ifstream &stream) override;
};

#endif
