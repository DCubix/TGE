#include "tgTextureAsset.h"

#include "../core/tgImageData.h"
#include "../graphics/tgTexture.h"

void* tgTextureAsset::load(PhysFS::ifstream &stream) {
	tgImageData dat(stream);
	return new tgTexture(dat, tgTexture::tgTEXTURE_RGBA);
}
