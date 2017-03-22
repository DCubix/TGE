#include "tgFontAsset.h"

#include "tgTextureAsset.h"

#include "../core/tgFont.h"
#include "../assets/tgAssets.h"

#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

static std::string getStrValue(std::string const& str) {
	std::string tmp = str.substr(str.find("=") + 1);
	tmp.erase(std::remove(tmp.begin(), tmp.end(), '"'), tmp.end());
	return tmp;
}

static int getIntValue(std::string const& str) {
	return atoi(getStrValue(str).c_str());
}

static std::vector<int> getArrValue(std::string const& str) {
	std::vector<std::string> vals = split(getStrValue(str), ',');
	std::vector<int> ivals;
	ivals.resize(vals.size());
	for (std::size_t i = 0; i < vals.size(); i++) {
		ivals[i] = atoi(vals[i].c_str());
	}
	return ivals;
}

void* tgFontAsset::load(PhysFS::ifstream &stream) {
	tgFont *fnt = new tgFont();
	float w = 1.0f;
	float h = 1.0f;
	
	std::regex re("\\S+='.*?'|\\S+=\".*?\"|\\S+");

	std::string line;
	std::vector<std::string> sm;
	while (std::getline(stream, line)) {
		sm.clear();
		std::smatch match;
		while (std::regex_search(line, match, re)) {
			if (!match.empty()) {
				sm.push_back(match[0]);
				line = match.suffix();
			}
		}

		std::string cmd = sm[0];

		if (cmd == "common") {
			w = float(getIntValue(sm[3]));
			h = float(getIntValue(sm[4]));
			fnt->m_mapWidth = w;
			fnt->m_mapHeight = h;
			fnt->m_lineHeight = float(getIntValue(sm[1]));
		} else if (cmd == "page") {
			std::string textureFile = getStrValue(sm[2]);
			tgTexture *tex = static_cast<tgTexture*>(tgAssets::loadSingle<tgTextureAsset>(textureFile));
			/*tex->setFilter(GL_NEAREST, GL_NEAREST);*/
			fnt->m_texture = tex;
		} else if (cmd == "char") {
			tgChar chr;
			chr.id = getIntValue(sm[1]);

			chr.clipRect.x() = float(getIntValue(sm[2]) / w);
			chr.clipRect.y() = float(getIntValue(sm[3]) / h);
			chr.clipRect.z() = float(getIntValue(sm[4]) / w);
			chr.clipRect.w() = float(getIntValue(sm[5]) / h);
			chr.xoffset = float(getIntValue(sm[6]));
			chr.yoffset = float(getIntValue(sm[7]));
			chr.xadvance = float(getIntValue(sm[8]));
			fnt->m_charMap[chr.id] = chr;
		} else if (cmd == "chars") {
			fnt->m_numChars = getIntValue(sm[1]);
		} else if (cmd == "info") {
			auto arr = getArrValue(sm[10]);
			fnt->m_padding.x() = float(arr[0]);
			fnt->m_padding.y() = float(arr[1]);
			fnt->m_padding.z() = float(arr[2]);
			fnt->m_padding.w() = float(arr[3]);
		}
	}
	return fnt;
}
