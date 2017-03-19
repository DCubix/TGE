#include "tgSpriteNode.h"

#include "../graphics/tgSpriteRenderer.h"

tgSpriteNode::tgSpriteNode(tgTexture *tex) : m_texture(tex) {}

tgSpriteNode::~tgSpriteNode() {
	tgDelete(m_texture);
	tgNode::~tgNode();
}

void tgSpriteNode::onRender(tgRenderer * ren) {
	tgSpriteRenderer *renderer = dynamic_cast<tgSpriteRenderer*>(ren);
	if(renderer && m_texture) {
		renderer->draw(m_texture,
			tgVector4(0, 0, 1, 1),
			tgVector4(getWorldPosition().xy(), 
						float(m_texture->getWidth()), float(m_texture->getHeight())),
			tgVector2(0.5f), 0.0f);
	}
}
