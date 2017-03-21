#include "tgSpriteNode.h"

tgSpriteNode::tgSpriteNode(tgTexture *tex) : m_texture(tex) {
	m_clipRectangle = tgVector4(0, 0, 1, 1);
	m_origin = tgVector2(0.5f);
}

tgSpriteNode::~tgSpriteNode() {
	
	tgNode::~tgNode();
}

void tgSpriteNode::onRender(tgRenderer * ren) {
	
}
