#ifndef FONT_COMPONENT_H
#define FONT_COMPONENT_H

#include "../ecs/tgComponent.h"
#include "../graphics/tgTexture.h"

#include <string>

class tgFontComponent : public tgComponent {
public:
private:
	std::string m_text;

};

#endif