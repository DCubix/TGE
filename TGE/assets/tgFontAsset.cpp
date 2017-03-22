#include "tgFontAsset.h"

#include "../core/tgFont.h"

#include <regex>
#include <sstream>
#include <algorithm>

static std::string getStrValue(std::string const& str) {
	std::string tmp = str.substr(str.find("=") + 1);
	std::remove(tmp.begin(), tmp.end(), '"');
	return tmp;
}

void* tgFontAsset::load(PhysFS::ifstream &stream) {
	tgFont *fnt = new tgFont();
	float w = 1.0f;
	float h = 1.0f;

	std::regex re("\\S+='.*?'|\\S+=\".*?\"|\\S+");

	std::string line;
	while (std::getline(stream, line)) {

	}
}
