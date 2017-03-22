#include "tgTextAsset.h"

#include <streambuf>

void* tgTextAsset::load(PhysFS::ifstream &stream) {
	return new std::string((std::istreambuf_iterator<char>(stream)),
							std::istreambuf_iterator<char>());
}
