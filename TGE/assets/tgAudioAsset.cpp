#include "tgAudioAsset.h"

#include "../audio/tgAudioBuffer.h"
#include "../audio/tgSampleData.h"

void* tgAudioAsset::load(PhysFS::ifstream& stream) {
	tgSampleData dat(stream);
	return new tgAudioBuffer(dat);
}
